#ifndef _CONFIGS_HPP
#define _CONFIGS_HPP

#include "../include/GL/glew.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"

namespace Configs
{
	struct d_s_color
	{
		float dcolor[3], scolor[3];
	};

	struct _parameters
	{
		int screenWidth;
		int screenHeight;
	};

	glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

	GLuint currentProgram, blitProgram, debugProgram, depthProgram, simpleProgram, shadowProgram, bgfxProgram, shadowMaskProgram;
	GLuint rttFramebuffer, rttTexture, depthTexture, shadowMaskTexture;
	int rttFramebuffer_width, rttFramebuffer_height;
	//int depthFramebuffer_width, depthFramebuffer_height;
	GLuint ltc_mat_texture, ltc_mag_texture;
	GLuint buffer, depthBuffer, lightRectBuffer, teapotBuffer, floorRectBuffer, shadowMaskBuffer;

	glm::vec3 cameraEyePos;
	//glm::vec3 LightCenter = glm::vec3(0, 9, -11); // For Render t16 Shadows


	int ymode = 3; // special mode for debug

	float eyez = 3.0;
	//float roty = 0.23, rotz = 0.082;
	float roty = 0.0, rotz = 0.0;
	float height = 8.0, width = 8.0;
	float lightIntensity = 4.0, roughness = 0.02;

	struct d_s_color dsColor = { { 1,1,1 },{ 1,1,1 } };
	struct _parameters parameters;

	bool twoSided = false;
	bool spin_mirror = false;
	int mode = 0;
	int frame, time_of_glut, timebase;
	double execTime = 0.0, fps = 0.0;
	float spin_height = 0.01, spin_width = 0.005;
	bool spin_mode_toggle = true, spin_hw_toggle = true;
	//ObjLoader teapot;
	float cameraPitch, cameraYaw, cameraDistance;
	GLubyte *p;
	float bias = 0.001;

	GLfloat pixels[512 * 512 * 3];


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

	//glm::vec3 LightCenter = glm::vec3(0, 6, 32); // For Original RenderScene
	//glm::vec3 LightCenter = glm::vec3(0, 16, 32); // ???
	//glm::vec3 LightCenter = glm::vec3(0, 9, -11); // For Render t16 Shadows
	glm::vec3 LightCenter = glm::vec3(0, 14, 6); // For bgfx debug

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


	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	GLuint quad_vertexbuffer;

	double xpos, ypos;
	float shadowBias = 0.001100001;

	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0, 3, eyez);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.00005f;

	float scale = 5;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
}

#endif // !_CONFIGS_HPP