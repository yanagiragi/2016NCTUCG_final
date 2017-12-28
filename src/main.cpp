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

#include "../include/shader.h"
#include "data.hpp"

#include "Reader.hpp"
#include "ObjLoader.hpp"

#define ROT_STEP 0.01
#define HEIGHT_WIDTH_STEP 0.5
#define LIGHT_INTENSITY_STEP 0.5
#define ROUGHNESS_STEP 0.01

void init(void);
void display(void);
void setClampedTextureState(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int uni_name, int y);
void mouseWheel(int wheel, int direction, int uni_name, int y);
void idle(void);

GLuint currentProgram, blitProgram, debugProgram, depthProgram, simpleProgram;
GLuint rttFramebuffer, rttTexture, depthTexture;
int rttFramebuffer_width, rttFramebuffer_height;
//int depthFramebuffer_width, depthFramebuffer_height;
GLuint ltc_mat_texture, ltc_mag_texture;
GLuint buffer, depthBuffer, lightRectBuffer, teapotBuffer;
struct _parameters 
{
	int screenWidth; 
	int screenHeight; 
}parameters;

std::time_t parameters_time;


float eyez = 3.0;
float roty = 0.23, rotz = 0.082;
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

glm::vec3 LightCenter = glm::vec3(0, 6, 32);

struct _demo_speed {
	int mode_counter;
	int speed = 3;
} demo_speed;

std::map<std::string, GLfloat*> g_params;
std::vector<const GLchar*> g_params_names = {
	"roty",
	"rotz",
	"height",
	"width",
	"intensity",
	"roughness",
};

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("2016CG final project 0556619_0556641 - LTC");
	glutReshapeWindow(512, 512);
	glewInit();

	/* line 33 js/super_slider.js */
	g_params["roty"] = &roty;
	g_params["rotz"] = &rotz;
	g_params["height"] = &height;
	g_params["width"] = &width;
	g_params["intensity"] = &lightIntensity;
	g_params["roughness"] = &roughness;

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

	glGenFramebuffers(1, &depthBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, rttFramebuffer_width, rttFramebuffer_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, rttFramebuffer_width, rttFramebuffer_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	setClampedTextureState();
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depthTexture, 0);	
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
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
	setClampedTextureState();

	glGenTextures(1, &ltc_mag_texture);
	glBindTexture(GL_TEXTURE_2D, ltc_mag_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_FLOAT, g_ltc_mag);
	setClampedTextureState();
	/* line - 283 */

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, NULL);

	float Camera_Pos_[3] = { 0, 3, 48 };
	cameraDistance = sqrtf(Camera_Pos_[0] * Camera_Pos_[0] + Camera_Pos_[1] * Camera_Pos_[1] + Camera_Pos_[2] * Camera_Pos_[2]);
}

void setClampedTextureState() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
float newLightRect[18];


void RenderSceneGeometry()
{
	glUseProgram(simpleProgram);

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	
	glDisable(GL_CULL_FACE);

	glm::vec3 Camera_Pos;
	Camera_Pos[0] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::cos(glm::radians(cameraYaw));
	Camera_Pos[1] = cameraDistance * glm::cos(glm::radians(cameraPitch));
	Camera_Pos[2] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::sin(glm::radians(cameraYaw));
	
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glPushMatrix();
	glTranslated(0.0, 3.0, 38);
	GLfloat view[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, view);
	glPopMatrix();

	glm::vec3 cameraPos = Camera_Pos;// glm::vec3(0.0, 3.0, 38 + 10);

	//glm::mat4 newView = glm::lookAt(cameraPos, cameraPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0, 1, 0));
	glm::mat4 newView = glm::lookAt(cameraPos, cameraPos + (LightCenter - cameraPos), glm::vec3(0, 1, 0));

	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "view"), 1, GL_FALSE, &newView[0][0]);

	glm::mat4 model(1.0f);
	
	model = glm::translate(model, LightCenter);
	model = glm::rotate(model, roty, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotz, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(width * 0.5, height * 0.5, 1));

	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "model"), 1, GL_FALSE, &model[0][0]);

	glm::mat4 proj = glm::perspective(glm::radians(45.0), 1.0, 0.0001, 1000.0);

	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "proj"), 1, GL_FALSE, &proj[0][0]);

	glUniform3f(glGetUniformLocation(simpleProgram, "color"), 1.0, 0.0, 0.0);

	GLuint vertexPositionLocation = glGetAttribLocation(simpleProgram, "position");

	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, newLightRect, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 25));
	
	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "model"), 1, GL_FALSE, &model[0][0]);

	glUniform3f(glGetUniformLocation(simpleProgram, "color"), 1.0, 1.0, 1.0);
	
	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, teapot.position.size() / 3);

	glDisableVertexAttribArray(vertexPositionLocation);

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

	/* line 583 - 593*/
	// Set values to program variables
	for (const GLchar* x : g_params_names) {
		float value = *(g_params[x]);
		GLuint loc = location(x);
		glUniform1f(loc, value);
	}
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(depthProgram);
	
	//glBindFramebuffer(GL_FRAMEBUFFER, rttFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec3 Camera_Pos;
	Camera_Pos[0] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::cos(glm::radians(cameraYaw));
	Camera_Pos[1] = cameraDistance * glm::cos(glm::radians(cameraPitch));
	Camera_Pos[2] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::sin(glm::radians(cameraYaw));

	glm::vec3 cameraPos = Camera_Pos;// glm::vec3(0.0, 3.0, 38 + 10);

	glm::mat4 model(1.0f);
	model = glm::translate(model, LightCenter);
	model = glm::rotate(model, roty, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotz, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(width * 0.5, height * 0.5, 1));

	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + (LightCenter - cameraPos), glm::vec3(0, 1, 0));

	glm::mat4 proj = glm::perspective(glm::radians(45.0), 1.0, 0.0001, 1000.0);

	glm::mat4 MVP = proj * view * model;

	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	GLuint vertexPositionLocation = glGetAttribLocation(depthProgram, "position");

	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 25));

	MVP = proj * view * model;

	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, teapot.position.size() / 3);

	glDisableVertexAttribArray(vertexPositionLocation);

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glUseProgram(NULL);
}

GLuint pixels[512 * 512 * 3];

void DebugRender()
{
	glUseProgram(debugProgram);
	
	GLuint tex = rttTexture;

	/* line 633 */
	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, GL_UNSIGNED_INT, pixels);

	int sum = 0;
	for (int i = 0; i < 512 * 512 * 3; ++i)
	{
		sum += pixels[i];
	}

	glUniform1i(glGetUniformLocation(debugProgram, "tex"), 0);
	glUniform2f(glGetUniformLocation(debugProgram, "resolution"), parameters.screenWidth, parameters.screenHeight);

	/* line 645 */
	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
	// Blit pass
	GLuint vertexPositionLocation = glGetAttribLocation(debugProgram, "position");
	glEnableVertexAttribArray(vertexPositionLocation);
	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void BiltRender()
{
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
	glEnableVertexAttribArray(vertexPositionLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
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

	//RenderSceneGeometry();

	//BiltRender();
	DebugRender();

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

	default: {break; }
	}
}
void mouseWheel(int wheel, int direction, int uni_name, int y) {
	if (direction > 0)	eyez += 1.0;
	else	eyez -= 1.0;
}
void idle(void) { glutPostRedisplay(); }