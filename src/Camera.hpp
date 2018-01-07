#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "Input.hpp"

glm::vec3 cameraEyePos;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

void computeMatricesFromInputs() {

	//position = glm::vec3(0, 3, eyez); // For Original Render Scene
	//position = glm::vec3(0, 6, eyez); // For Original Render Scene (Adjusted)
	//position = glm::vec3(0, 10, eyez); // For Render t16 Shadow
	position = glm::vec3(0, 10, eyez); // For debug bgfx
	cameraEyePos = position;
	//position = glm::vec3(14,6,4);

	// Compute time difference between current and last frame
	double currentTime = time_of_glut;
	float deltaTime = float(currentTime - timebase);
	
	// Reset mouse position for next frame
	glutWarpPointer(512 / 2, 512 / 2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(512 / 2 - xpos);
	verticalAngle += mouseSpeed * float(512 / 2 - ypos);

	/*horizontalAngle = 0.0199888;
	verticalAngle = 6.45002;
	eyez = -25;*/

	//horizontalAngle = 3.1456;
	//verticalAngle = -0.099;
	//eyez = 12;

	// For bgfx
	//horizontalAngle = -0.0129214;
	//verticalAngle = 0.0402145;
	eyez = -19;

	//eyez = 51;
	
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
		position,				// Camera is here
		position + direction,	// and looks here : at the same position, plus "direction"
		up						// Head is up (set to 0,-1,0 to look upside-down)
	);

}