#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <string>
#include <map>
#include <cmath>
#include <ctime>

#include "../include/GL/glew.h"
#include "../include/GL/freeglut.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "../include/shader.h"

#include "data.hpp"
#include "Reader.hpp"
#include "ObjLoader.hpp"

#include "tutorial16.hpp"

#define ROT_STEP 0.01
#define HEIGHT_WIDTH_STEP 0.5
#define LIGHT_INTENSITY_STEP 0.5
#define ROUGHNESS_STEP 0.01

void init(void);
void display(void);
/*void setClampedTextureState(void);*/
void reshape(int width, int height);
void keyboard(unsigned char key, int uni_name, int y);
void mouseWheel(int wheel, int direction, int uni_name, int y);
void idle(void);

GLuint currentProgram, blitProgram, debugProgram, depthProgram, simpleProgram;
GLuint rttFramebuffer, rttTexture, depthTexture;
int rttFramebuffer_width, rttFramebuffer_height;
//int depthFramebuffer_width, depthFramebuffer_height;
GLuint ltc_mat_texture, ltc_mag_texture;
GLuint buffer, depthBuffer, lightRectBuffer, teapotBuffer, floorRectBuffer;
struct _parameters 
{
	int screenWidth; 
	int screenHeight; 
}parameters;

std::time_t parameters_time;

int ymode = 0; // special mode for debug

float eyez = 3.0;
//float roty = 0.23, rotz = 0.082;
float roty = 0.0, rotz = 0.0;
float height = 8.0, width = 8.0;
float lightIntensity = 4.0, roughness = 0.02;
struct d_s_color { float dcolor[3], scolor[3]; };
d_s_color dsColor = { { 1,1,1 },{ 1,1,1 } };
bool twoSided = false;
bool spin_mirror = false;
int mode = 0;
int frame, time_of_glut, timebase;
double execTime = 0.0, fps = 0.0;
float spin_height = 0.01, spin_width = 0.005;
bool spin_mode_toggle = true, spin_hw_toggle = true;
ObjLoader teapot;
float cameraPitch, cameraYaw, cameraDistance;
GLubyte *p;

GLuint location(const GLchar* u) 
{ 
	// FIXED PROGRAM
	return glGetUniformLocation(currentProgram, u); 
}

float lightRect[] = {
	-1.0, -1.0,  0.0,
	 1.0, -1.0,  0.0, 
	-1.0,  1.0,  0.0,
	 1.0, -1.0,  0.0,
	 1.0,  1.0,  0.0,
	-1.0,  1.0,  0.0
};

float FloorRect[] = {
	-1.0, 0.0, -1.0,
	 1.0, 0.0, -1.0,
	-1.0, 0.0,  1.0,
	 1.0, 0.0, -1.0,
	 1.0, 0.0,  1.0,
	-1.0, 0.0,  1.0
};

glm::vec3 LightCenter = glm::vec3(0, 6, 32);
float newLightRect[18];

struct _demo_speed {
	int mode_counter;
	int speed = 3;
} demo_speed;

//std::map<std::string, GLfloat*> g_params;
//std::vector<const GLchar*> g_params_names = {
//	"roty",
//	"rotz",
//	"height",
//	"width",
//	"intensity",
//	"roughness",
//};

tutorial16 t16;
static const GLfloat g_quad_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
};

GLuint quad_vertexbuffer;

void screenshot_ppm_DepthPow(const char *filename, unsigned int width, unsigned int height, GLfloat *pixels, GLuint tex, float powFactor) {
	size_t i, j, k, cur;
	const size_t format_nchannels = 1;
	FILE *f = fopen(filename, "w");
	fprintf(f, "P3\n%d\n%d\n%d\n", width, height, 255);
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
	glGetTextureImage(tex, 0, GL_DEPTH_COMPONENT, GL_FLOAT, sizeof(GLfloat) * width * height, pixels);
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			cur = format_nchannels * ((height - i - 1) * width + j);
			float p = pow((pixels)[cur], powFactor);
			int pInt = p * 255.0;
			fprintf(f, "%3d %3d %3d ", pInt, pInt, pInt);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

void screenshot_ppm_Depth(const char *filename, unsigned int width, unsigned int height, GLfloat *pixels, GLuint tex) {
	size_t i, j, k, cur;
	const size_t format_nchannels = 1;
	FILE *f = fopen(filename, "w");
	fprintf(f, "P3\n%d\n%d\n%d\n", width, height, 255);
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
	glGetTextureImage(tex, 0, GL_DEPTH_COMPONENT, GL_FLOAT, sizeof(GLfloat) * width * height, pixels);
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			cur = format_nchannels * ((height - i - 1) * width + j);
			float p = (pixels)[cur];
			int pInt = p * 255.0;
			fprintf(f, "%3d %3d %3d ", pInt, pInt, pInt);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

void screenshot_ppm_Depth(const char *filename, unsigned int width, unsigned int height, GLfloat *pixels) {
	size_t i, j, k, cur;
	const size_t format_nchannels = 1;
	FILE *f = fopen(filename, "w");
	fprintf(f, "P3\n%d\n%d\n%d\n", width, height, 255);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			cur = format_nchannels * ((height - i - 1) * width + j);
			float p = (pixels)[cur];
			int pInt = p * 255.0;
			fprintf(f, "%3d %3d %3d ", pInt, pInt, pInt);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

void screenshot_ppm_RGB(const char *filename, unsigned int width, unsigned int height, GLubyte *pixels) {
	size_t i, j, k, cur;
	const size_t format_nchannels = 3;
	FILE *f = fopen(filename, "w");
	fprintf(f, "P3\n%d\n%d\n%d\n", width, height, 255);
	pixels = (GLubyte*)realloc(pixels, format_nchannels * sizeof(GLubyte) * width * height);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	int sum = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			cur = format_nchannels * ((height - i - 1) * width + j);
			fprintf(f, "%3d %3d %3d ", (pixels)[cur], (pixels)[cur + 1], (pixels)[cur + 2]);
			sum += (int)(pixels)[cur] + (int)(pixels)[cur + 1] + (int)(pixels)[cur + 2];
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("2016CG final project 0556619_0556641 - LTC");
	glutReshapeWindow(512, 512);
	glewInit();

	///* line 33 js/super_slider.js */
	//g_params["roty"] = &roty;
	//g_params["rotz"] = &rotz;
	//g_params["height"] = &height;
	//g_params["width"] = &width;
	//g_params["intensity"] = &lightIntensity;
	//g_params["roughness"] = &roughness;

	init();

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("GLSL version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseWheelFunc(mouseWheel);

	timebase = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();

	return 0;
}

void init(void) {
	/* line 171 */
	GLuint vertex_shader = createShader("shaders/ltc/ltc.vs", "vertex");
	GLuint fragment_shader = createShader("shaders/ltc/ltc.fs", "fragment");

	/* line 216 */
	// Create Vertex buffer (2 triangles)
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	float floor[] = {
		-1.0, -1.0,
		 1.0, -1.0,
		-1.0,  1.0,
		 1.0, -1.0,
		 1.0,  1.0,
		 -1.0,  1.0
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, floor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glGenBuffers(1, &floorRectBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, floorRectBuffer);

	// Static for Now
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, FloorRect, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);


	glGenBuffers(1, &lightRectBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
	
	// Static for Now
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, lightRect, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	teapot = ObjLoader("models/teapot.obj");

	glGenBuffers(1, &teapotBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * teapot.position.size(), &(teapot.position[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	/* line 220 */
	// Create Program
	currentProgram = createProgram(vertex_shader, fragment_shader);

	/* line 224 */
	//glCreateFramebuffers(1,&rttFramebuffer); // just for OpenGL 4.5
	glGenFramebuffers(1, &rttFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, rttFramebuffer);

	rttFramebuffer_width = 512; // FIXME
	rttFramebuffer_height = 512;

	//glCreateTextures(GL_TEXTURE_2D,1,&rttTexture); // just for OpenGL 4.5
	glGenTextures(1, &rttTexture);
	glBindTexture(GL_TEXTURE_2D, rttTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rttFramebuffer_width, rttFramebuffer_height, 0, GL_RGBA, GL_FLOAT, NULL); // Note: use GL_RGBA32F instead of GL_RGBA
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rttTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);

	glBindTexture(GL_TEXTURE_2D, NULL);

	/*
	*	Generate Depth Map
	*/

	glGenFramebuffers(1, &depthBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rttFramebuffer_width, rttFramebuffer_height, 0, GL_RGBA, GL_FLOAT, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthTexture, 0);;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, rttFramebuffer_width, rttFramebuffer_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FrameBuffer Not Ready" << std::endl;


	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);

	
	/* line 239 - 256*/
	GLuint blit_vs = createShader("shaders/ltc/ltc_blit.vs", "vertex");
	GLuint blit_fs = createShader("shaders/ltc/ltc_blit.fs", "fragment");
	blitProgram = createProgram(blit_vs, blit_fs);

	GLuint simple_vs = createShader("shaders/simple.vs", "vertex");
	GLuint simple_fs = createShader("shaders/simple.fs", "fragment");
	simpleProgram = createProgram(simple_vs, simple_fs);

	GLuint shadowMap_vs = createShader("shaders/shadowMap.vs", "vertex");
	GLuint shadowMap_fs = createShader("shaders/shadowMap.fs", "fragment");
	depthProgram = createProgram(shadowMap_vs, shadowMap_fs);

	GLuint debug_vs = createShader("shaders/Debug.vs", "vertex");
	GLuint debug_fs = createShader("shaders/Debug.fs", "fragment");
	debugProgram = createProgram(debug_vs, debug_fs);

	/* line 275 - */
	glGenTextures(1, &ltc_mat_texture);
	glBindTexture(GL_TEXTURE_2D, ltc_mat_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 64, 64, 0, GL_RGBA, GL_FLOAT, g_ltc_mat); // Note: use GL_RGBA32F instead of GL_RGBA
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &ltc_mag_texture);
	glBindTexture(GL_TEXTURE_2D, ltc_mag_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_FLOAT, g_ltc_mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* line - 283 */

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, NULL);

	float Camera_Pos_[3] = { 0, 3, 48 };
	cameraDistance = sqrtf(Camera_Pos_[0] * Camera_Pos_[0] + Camera_Pos_[1] * Camera_Pos_[1] + Camera_Pos_[2] * Camera_Pos_[2]);


	// For Tutorial16
	t16 = tutorial16();
	t16.loadOBJ("models/room_thickwalls2.obj", t16.vertices, t16.uvs, t16.normals);

	
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

//void setClampedTextureState() 
//{
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//}

void RenderSceneGeometry()
{
	glUseProgram(simpleProgram);

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	
	glDisable(GL_CULL_FACE);

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	/*glm::vec3 Camera_Pos;
	Camera_Pos[0] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::cos(glm::radians(cameraYaw));
	Camera_Pos[1] = cameraDistance * glm::cos(glm::radians(cameraPitch));
	Camera_Pos[2] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::sin(glm::radians(cameraYaw));*/
	
	GLuint vertexPositionLocation = glGetAttribLocation(simpleProgram, "position");

	glm::vec3 cameraPos = glm::vec3(0, 3, -3);
	//glm::vec3 cameraPos = glm::vec3(0, 10, -6); // another view for debugging

	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(LightCenter.x, cameraPos.y, LightCenter.z), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "view"), 1, GL_FALSE, &view[0][0]);

	//glm::mat4 proj = glm::perspective(glm::radians(45.0), 1.0, 0.0001, 1000.0);
	glm::mat4 proj = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);
	//glm::mat4 proj = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "proj"), 1, GL_FALSE, &proj[0][0]);
	
	glm::mat4 proj_depth = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);//glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 view_depth = glm::lookAt(LightCenter, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "proj_depth"), 1, GL_FALSE, &proj_depth[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "view_depth"), 1, GL_FALSE, &view_depth[0][0]);

	glm::mat4 model(1.0f);
	
	/*
	*	Draw the Floor
	*/

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::scale(model, glm::vec3(50, 1, 90));

	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glUniform3f(glGetUniformLocation(simpleProgram, "color"), 0.0, 1.0, 0.0);	
	glBindBuffer(GL_ARRAY_BUFFER, floorRectBuffer);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	/*
	*	Draw the Teapot
	*/
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	model = glm::mat4(1.0f);
	// model = glm::translate(model, glm::vec3(0, 0, 15));
	model = glm::translate(model, glm::vec3(10, 0, 33 + roty));
	model = glm::scale(model, glm::vec3(1, 1, 1) * 5.0f);

	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "model"), 1, GL_FALSE, &model[0][0]);

	glUniform3f(glGetUniformLocation(simpleProgram, "color"), 0.0, 0.0, 1.0);
	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, teapot.position.size() / 3);

	/*
	*	Draw the Light Rect
	*/
	
	model = glm::mat4(1.0f);
	// Light Center = vec3(0, 6, 32);
	model = glm::translate(model, LightCenter);


	/*
	vec3 rotation_y(vec3 v, float a){return vec3(v.x*cos(a) + v.z*sin(a), v.y, -v.x*sin(a) + v.z*cos(a));}
	vec3 rotation_z(vec3 v, float a){return vec3(v.x*cos(a) - v.y*sin(a), v.x*sin(a) + v.y*cos(a), v.z);}
	vec3 rotation_yz(vec3 v, float ay, float az){return rotation_z(rotation_y(v, ay), az);} 
	*/

	/*
	glm::vec3 v = glm::vec3(0, 1, 0);
	v = glm::vec3(v.x*cos(roty) + v.z*sin(roty), v.y, -v.x*sin(roty) + v.z*cos(roty));
	glm::vec3 rotAxisAfterYRotated = glm::vec3(v.x*cos(roty) - v.y*sin(roty), v.x*sin(roty) + v.y*cos(roty), v.z);
	model = glm::rotate(model, -roty * 2.0f * 3.14f, v);

	v = glm::vec3(0, 0, 1);
	v = glm::vec3(v.x*cos(rotz) - v.y*sin(rotz), v.x*sin(rotz) + v.y*cos(rotz), v.z);
	*/
	
	model = glm::rotate(model, -roty * 2.0f * 3.14f, glm::vec3(0, 1, 0));
	model = glm::rotate(model, -rotz * 2.0f * 3.14f, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(width * 0.5, height * 0.5, 1));

	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glUniform3f(glGetUniformLocation(simpleProgram, "color"), 1.0, 0.0, 0.0);

	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(vertexPositionLocation);
	glDisable(GL_BLEND);
}

void RenderScene()
{
	// Load program into GPU
	glUseProgram(currentProgram);
	/* line - 566*/

	glMatrixMode(GL_MODELVIEW);
	/* line 571*/
	glLoadIdentity();
	glPushMatrix();
	glTranslated(0.0, 3.0, eyez);
	GLfloat view[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, view);
	glPopMatrix();

	/* line 577*/
	// Get var locations
	GLuint vertexPositionLocation = glGetAttribLocation(currentProgram, "position");

	/* line 583 - 593 */
	// Set values to program variables
	//for (const GLchar* x : g_params_names) {
	//	float value = *(g_params[x]);
	//	GLuint loc = location(x);
	//	glUniform1f(loc, value);
	//}

	glUniform1f(location("roty"), roty);
	glUniform1f(location("rotz"), rotz);
	glUniform1f(location("height"), height);
	glUniform1f(location("width"), width);
	glUniform1f(location("intensity"), lightIntensity);
	glUniform1f(location("roughness"), roughness);

	glUniform1i(location("twoSided"), twoSided);
	glUniform3f(location("dcolor"), dsColor.dcolor[0], dsColor.dcolor[1], dsColor.dcolor[2]);
	glUniform3f(location("scolor"), dsColor.scolor[0], dsColor.scolor[1], dsColor.scolor[2]);
	glUniform1i(location("mode"), mode);

	/* line 594 - 596*/
	glUniformMatrix4fv(location("view"), 1, GL_FALSE, view);
	glUniform2f(location("resolution"), parameters.screenWidth, parameters.screenHeight);

	/* line 598 - */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ltc_mat_texture);
	glUniform1i(glGetUniformLocation(currentProgram, "ltc_mat"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ltc_mag_texture);
	glUniform1i(glGetUniformLocation(currentProgram, "ltc_mag"), 1);
	/* line - 604 */

	glUniform3f(glGetUniformLocation(currentProgram, "LightCenter"), LightCenter.x, LightCenter.y, LightCenter.z);

	/* line 608 -  */
	glBindFramebuffer(GL_FRAMEBUFFER, rttFramebuffer);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	/* line - 619 */

	/* line 621 - */
	// Render geometry
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(vertexPositionLocation, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(vertexPositionLocation);


	/* line 628 */
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

void RenderShadowMap()
{
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/

	/*glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);*/

	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
	// glBindFramebuffer(GL_FRAMEBUFFER, NULL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(depthProgram);

	glm::vec3 Camera_Pos;
	Camera_Pos[0] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::cos(glm::radians(cameraYaw));
	Camera_Pos[1] = cameraDistance * glm::cos(glm::radians(cameraPitch));
	Camera_Pos[2] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::sin(glm::radians(cameraYaw));

	glm::vec3 cameraPos = glm::vec3(0.0, 3.0, -3);
	
	glm::mat4 model(1.0f);
	model = glm::translate(model, LightCenter);
	model = glm::rotate(model, -roty * 2.0f * 3.14f, glm::vec3(0, 1, 0));
	model = glm::rotate(model, -rotz * 2.0f * 3.14f, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(width * 0.5, height * 0.5, 1));

	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(LightCenter.x, cameraPos.y, LightCenter.z), glm::vec3(0, 1, 0));
	
	float w = 14.35;
	glm::mat4 proj = glm::ortho<float>(-w, w, -w, w, -1000, 2000);
	proj = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);

	glm::mat4 MVP = proj * view * model;

	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	GLuint vertexPositionLocation = glGetAttribLocation(depthProgram, "position");	

	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10, 0, 33 + roty));
	model = glm::scale(model, glm::vec3(1, 1, 1) * 5.0f);

	MVP = proj * view * model;

	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, teapot.position.size() / 3);

	glDisableVertexAttribArray(vertexPositionLocation);

	/*view = glm::make_mat4(viewMatrix);

	MVP = proj * view * model;
	MVP = view;

	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(vertexPositionLocation, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(vertexPositionLocation);*/

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glUseProgram(NULL);
}

GLfloat pixels[512 * 512];

void DebugRender()
{
	glUseProgram(debugProgram);
	
	//GLuint tex = rttTexture;
	GLuint tex = depthTexture;

	screenshot_ppm_DepthPow("output.ppm", 512, 512, pixels, depthTexture, 100000);

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	//glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);

	//float sum = 0;
	//for (int i = 0; i < 512 * 512; ++i)
	//{
	//	sum += pixels[i];
	//}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glUniform1i(glGetUniformLocation(debugProgram, "tex"), 0);
	glUniform2f(glGetUniformLocation(debugProgram, "resolution"), parameters.screenWidth, parameters.screenHeight);
	
	GLuint vertexPositionLocation = 0;
	/* line 645 */
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// Blit pass
	glEnableVertexAttribArray(vertexPositionLocation);
	glVertexAttribPointer(vertexPositionLocation, 2, GL_FLOAT, false, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(vertexPositionLocation);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

}

void BiltRender()
{
	//FBO_2_PPM_file(512, 512);
	GLuint vertexPositionLocation = glGetAttribLocation(currentProgram, "position");

	glUseProgram(blitProgram);
	glDisable(GL_BLEND);

	GLuint tex = rttTexture;

	/* line 633 */
	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(glGetUniformLocation(blitProgram, "tex"), 0);
	/* line 638 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glUniform2f(glGetUniformLocation(blitProgram, "resolution"), parameters.screenWidth, parameters.screenHeight);

	/* line 645 */
	// Blit pass
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	
	glEnableVertexAttribArray(vertexPositionLocation);
	glVertexAttribPointer(vertexPositionLocation, 2, GL_FLOAT, false, 0, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	return;
}

void draw() {
	/* line 555 - */
	parameters_time = std::clock();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);

	// Note: the viewport is automatically set up to cover the entire Canvas.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//RenderScene();
	RenderShadowMap();
	//t16.RenderShadowMap16(depthBuffer, depthProgram);
	
	//BiltRender();
	DebugRender();

	/*if (ymode == 0) {
		RenderSceneGeometry();
	}
	else if (ymode == 1) {
		RenderScene();
		BiltRender();
	}*/

	glDisableVertexAttribArray(glGetAttribLocation(currentProgram, "position"));
	glBindTexture(GL_TEXTURE_2D, NULL);
	glBindVertexArray(NULL);
	glUseProgram(NULL);


	frame++;
	time_of_glut = glutGet(GLUT_ELAPSED_TIME);
	if (time_of_glut - timebase > 1000) {
		fps = frame / ((double)(time_of_glut - timebase) / 1000.0);
		execTime = 1.0 / fps * 1000;
		timebase = time_of_glut;
		frame = 0;
		if (spin_mirror) {
			if(spin_mode_toggle)
				++demo_speed.mode_counter;
			if (demo_speed.mode_counter > 1) {
				mode = (mode + 1) % 3;
				demo_speed.mode_counter = 0;
			}
		}
	}
}

void display(void) {
	if (spin_mirror) {
		roty += 0.005 / demo_speed.speed;
		rotz += 0.008 / demo_speed.speed;
		if (spin_hw_toggle) {
			height += spin_height;
			width += spin_width;
		}
		if (height > 14 || height < 1)
			spin_height *= -1;
		if (width > 14 || width < 1)
			spin_width *= -1;
	}

	draw();
	if (mode == 1)
		printf("\r 8 edges: %.lfFPS (%.2lf ms/frame)", fps, execTime);
	else if (mode == 2)
		printf("\r12 edges: %.lfFPS (%.2lf ms/frame)", fps, execTime);
	else
		printf("\r 4 edges: %.lfFPS (%.2lf ms/frame)", fps, execTime);
	glFinish();
}

void reshape(int width, int height) {
	parameters.screenWidth = width;
	parameters.screenHeight = height;
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int uni_name, int y) {
	switch (key) {
	case 27: {	/* ESC */ exit(0); break; }
	case 'w': {rotz += ROT_STEP; break; }
	case 's': {rotz -= ROT_STEP; break; }
	case 'a': {roty -= ROT_STEP; break; }
	case 'd': {roty += ROT_STEP; break; }
	case 't': {if (height < 15) { height += HEIGHT_WIDTH_STEP; }break; }
	case 'g': {if (height > 1) { height -= HEIGHT_WIDTH_STEP; }break; }
	case 'f': {if (width > 1) { width -= HEIGHT_WIDTH_STEP; }break; }
	case 'h': {if (width < 15) { width += HEIGHT_WIDTH_STEP; }break; }
	case 'j': {if (lightIntensity > 0.1) { lightIntensity -= LIGHT_INTENSITY_STEP; }
			  else { lightIntensity /= 2; } break; }
	case 'k': {if (lightIntensity < 9.5) { lightIntensity += LIGHT_INTENSITY_STEP; }break; }
	case 'u': {if (roughness > 0.03) { roughness -= ROUGHNESS_STEP; }break;	}
	case 'i': {if (roughness < 9.97) { roughness += ROUGHNESS_STEP; }break;	}
	case 'z': {twoSided = !twoSided;	break; }
	case 'q': {spin_mirror = !spin_mirror; break; }
	case 'e': {roty = 0.0; rotz = 0.0; height = 8.0; width = 8.0; break; }
	case 'm': {mode = (mode + 1) % 3; break; }
	case 'o': {if (demo_speed.speed < 10) { ++demo_speed.speed; }break; }
	case 'p': {if (demo_speed.speed > 1) { --demo_speed.speed; }break; }
	case '[': {spin_mode_toggle = !spin_mode_toggle; break; }
	case ']': {spin_hw_toggle = !spin_hw_toggle; break; }
	
	case '.': {
		std::cout << cameraPitch << std::endl; 
		cameraPitch += 1; break; 
	}
	case '/': {
		std::cout << cameraPitch << std::endl;
		cameraPitch -= 1; break; 
	}
	case ';': {
		std::cout << cameraYaw << std::endl;
		cameraYaw += 1; break;
	}
	case '\'': {
		std::cout << cameraYaw << std::endl;
		cameraYaw -= 1; break;
	}

	case 'x': {
		ymode++;
		ymode = ymode % 2;
		break;
	}
	
	case '1': {dsColor.dcolor[0] = dsColor.dcolor[1] = dsColor.dcolor[2] = 1; break; }
	case '2': {dsColor.dcolor[0] = 0; break; }
	case '3': {dsColor.dcolor[1] = 0; break; }
	case '4': {dsColor.dcolor[2] = 0; break; }
	case '5': {dsColor.scolor[0] = dsColor.scolor[1] = dsColor.scolor[2] = 1; break; }
	case '6': {dsColor.scolor[0] = 0; break; }
	case '7': {dsColor.scolor[1] = 0; break; }
	case '8': {dsColor.scolor[2] = 0; break; }
	case '9': {break; }
	case '0': {dsColor = { { 1,1,1 },{ 1,1,1 } }; break; }

	case '\\':
	{
		screenshot_ppm_RGB("output.ppm", 512, 512, p);
		break;
	}

	default: {break; }
	}
}
void mouseWheel(int wheel, int direction, int uni_name, int y) {
	if (direction > 0)	eyez += 1.0;
	else	eyez -= 1.0;
}
void idle(void) { glutPostRedisplay(); }