#ifndef _INPUT_HPP
#define _INPUT_HPP

#include "tutorial16.hpp"

#include "../include/GL/glew.h"

#define ROT_STEP 0.01
#define HEIGHT_WIDTH_STEP 0.5
#define LIGHT_INTENSITY_STEP 0.5
#define ROUGHNESS_STEP 0.01

struct d_s_color 
{ 
	float dcolor[3], scolor[3]; 
};

struct _parameters
{
	int screenWidth;
	int screenHeight;
};

namespace 
{
	glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

	std::time_t parameters_time;

	int ymode = 0; // special mode for debug

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
	ObjLoader teapot;
	float cameraPitch, cameraYaw, cameraDistance;
	GLubyte *p;

	GLfloat pixels[512 * 512];


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

	//glm::vec3 LightCenter = glm::vec3(0, 6, 32);
	//glm::vec3 LightCenter = glm::vec3(0, 16, 32);
	glm::vec3 LightCenter = glm::vec3(0.5f, 2, 2);
	
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

	double xpos, ypos;
	float shadowBias = 0.000100001;
	
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

void mouse(int x, int y)
{
	xpos = x;
	ypos = y;
}


void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		shadowBias += 0.0001;
		break;
	case GLUT_KEY_DOWN:
		shadowBias -= 0.0001;
		break;
	case GLUT_KEY_LEFT:
		scale += 0.1;
		break;
	case GLUT_KEY_RIGHT:
		scale -= 0.1;
		break;
	}
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
			  //case 'm': {mode = (mode + 1) % 3; break; }
	case 'o': {if (demo_speed.speed < 10) { ++demo_speed.speed; }break; }
	case 'p': {if (demo_speed.speed > 1) { --demo_speed.speed; }break; }
	case '[': {spin_mode_toggle = !spin_mode_toggle; break; }
	case ']': {spin_hw_toggle = !spin_hw_toggle; break; }

	case 'c': {
	LightCenter.x += 1; break;
	}
	case 'v': {
	LightCenter.x -= 1; break;
	}
	case 'b': {
	LightCenter.y += 1; break;
	}
	case 'n': {
	LightCenter.y -= 1; break;
	}
	case 'm': {
	LightCenter.z += 1;break;
	}
	case '/': {
	LightCenter.z -= 1;
	break;
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
		std::cout
			<< "\n========================================"
			<< "\nCamera eyeZ = " << eyez
			<< "\nCamera horizontalAngle = " << horizontalAngle
			<< "\nCamera verticalAngle = " << verticalAngle
			<< "\nShadow Bias = " << shadowBias
			<< "\nScale = " << scale
			<< "\n Light Center = (" << LightCenter.x << ", " << LightCenter.y << ", " << LightCenter.z << ")"
			<< "\n========================================\n" << std::endl;

		//screenshot_ppm_RGB("output.ppm", 512, 512, p);
		break;
	}

	default: {break; }
	}
}

void mouseWheel(int wheel, int direction, int uni_name, int y) {
	if (direction > 0)	eyez += 1.0;
	else	eyez -= 1.0;
}
#endif