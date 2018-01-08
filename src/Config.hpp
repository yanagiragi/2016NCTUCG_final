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

	struct _demo_speed {
		int mode_counter;
		int speed = 3;
	} demo_speed;

	struct d_s_color dsColor = { { 1,1,1 },{ 1,1,1 } };
	struct _parameters parameters;

	int rttFramebuffer_width, rttFramebuffer_height;
	GLuint ltcProgram, blitProgram, debugProgram, depthProgram, shadowProgram, bgfxProgram, shadowMaskProgram;
	GLuint rttFramebuffer, rttTexture, depthTexture, shadowMaskTexture;
	GLuint ltc_mat_texture, ltc_mag_texture;
	GLuint screenBuffer, depthBuffer, lightRectBuffer, teapotBuffer, shadowMaskBuffer;

	int ymode = 0; // special mode for debug

	float eyez = 3.0;
	float roty = 0.0, rotz = 0.0;
	float height = 8.0, width = 8.0;
	float lightIntensity = 4.0, roughness = 0.02;
	bool twoSided = false;
	bool spin_mirror = false;
	int mode = 0;
	int frame, time_of_glut, timebase;
	double execTime = 0.0, fps = 0.0;
	float spin_height = 0.01, spin_width = 0.005;
	bool spin_mode_toggle = true, spin_hw_toggle = true;

	float shadowBias = 0.001100001;

	// For Utils::screenshot
	GLubyte *p;
	// GLfloat pixels[512 * 512 * 3];

	glm::vec3 LightCenter = glm::vec3(0, 15, 6); // For bgfx debug

												 /* Camera Variables */
												 // Initial position : on +Z
	glm::vec3 cameraEyePos = glm::vec3(0, 3, eyez);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.00005f;

	// For Mouse input
	double xpos, ypos;
}

#endif // !_CONFIGS_HPP