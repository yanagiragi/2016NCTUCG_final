#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "Input.hpp"

glm::mat4 getViewMatrix() {
	return Configs::ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return Configs::ProjectionMatrix;
}

void computeMatricesFromInputs() {
	using namespace Configs;
	//position = glm::vec3(0, 3, eyez); // For Original Render Scene
	//position = glm::vec3(0, 6, eyez); // For Original Render Scene (Adjusted)
	//position = glm::vec3(0, 10, eyez); // For Render t16 Shadow
	//position // For debug bgfx
	cameraEyePos = glm::vec3(0, 10, eyez);;
	//position = glm::vec3(14,6,4);

	// Compute time difference between current and last frame
	double currentTime = time_of_glut;
	float deltaTime = float(currentTime - timebase);
	
	// Reset mouse position for next frame
	glutWarpPointer(512 / 2, 512 / 2);

	// Compute new orientation
	if (horizontalAngle > 6.089 && horizontalAngle < 6.4208)
	{
		horizontalAngle += mouseSpeed * float(512 / 2 - xpos);
	}
	else
	{
		horizontalAngle = Utils::clamp(horizontalAngle, 6.090, 6.4207);
	}

	if (verticalAngle > -0.302 && verticalAngle < 0.102) 
	{
		verticalAngle += mouseSpeed * float(512 / 2 - ypos);
	}
	else 
	{
		verticalAngle = Utils::clamp(verticalAngle, -0.301, 0.101);
	}
	
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	float FoV = initialFoV;

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 1.0f, 0.0008f, 1000.0f);	// Camera matrix
	
	ViewMatrix = glm::lookAt(
		cameraEyePos,				// Camera is here
		cameraEyePos + direction,	// and looks here : at the same position, plus "direction"
		up							// Head is up (set to 0,-1,0 to look upside-down)
	);

}

#endif