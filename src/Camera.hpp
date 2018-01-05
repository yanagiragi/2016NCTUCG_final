#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "Input.hpp"

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

void computeMatricesFromInputs() {

	// glfwGetTime is called only once, the first time this function is called
	//static double lastTime = glfwGetTime();
	//position = glm::vec3(0, 3, eyez);
	position = glm::vec3(0, 10, eyez);
	//position = glm::vec3(14,6,4);

	// Compute time difference between current and last frame
	double currentTime = time_of_glut;
	float deltaTime = float(currentTime - timebase);
	//float deltaTime = 0.001;

	// Get mouse position
	//glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	// glfwSetCursorPos(window, 1024/2, 768/2);
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

	// Move forward
	/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
	position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
	position -= right * deltaTime * speed;
	}*/

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

						   // Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 1.0f, 0.0008f, 1000.0f);	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,				// Camera is here
		position + direction,	// and looks here : at the same position, plus "direction"
		up						// Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	//lastTime = currentTime;
}