// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.


#include "controls.hpp"


Controls::Controls(GLFWwindow* window) :
	window(window),
	position(glm::vec3(-2.5, 3.1, -2.5)),
	horizontalAngle(3.141592 / 4),
	verticalAngle(-3.141592 / 4),
	initialFoV(45.0f),
	speed(5.0f),
	mouseSpeed(0.001f),
	ViewMatrix(),
	ProjectionMatrix(),
	radius(0)
{
	int xsize, ysize;
	glfwGetWindowSize(window, &xsize, &ysize);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, xsize / 2, ysize / 2);
}

void Controls::setCoord(float NS, float EW) {
	horizontalAngle = 3.141593 + 3.141593 * (EW / 180.0);
	verticalAngle = -3.141593 * (NS / 180.0);
}

float Controls::getTime() {
	return dt;
}

glm::mat4 Controls::getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 Controls::getProjectionMatrix() {
	return ProjectionMatrix;
}


void Controls::computeMatricesFromInputs(glm::vec3& objectPos) {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	dt = deltaTime;

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	int xsize, ysize;
	glfwGetWindowSize(window, &xsize, &ysize);

	if (active) {
		// Reset mouse position for next frame
		glfwSetCursorPos(window, xsize / 2, xsize / 2);

		// Compute new orientation
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			horizontalAngle += mouseSpeed * float(xsize / 2 - xpos) * (radius - 1.0);
			verticalAngle += mouseSpeed * float(xsize / 2 - ypos) * (radius - 1.0);
		}
	}
	if (verticalAngle > 1.5) verticalAngle = 1.5;
	else if (verticalAngle < -1.5) verticalAngle = -1.5;


	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.1416f / 2.0f),
		0,
		cos(horizontalAngle - 3.1416f / 2.0f)
	);


	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	if (active) {
		// escape 
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			active = 0;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			float zoom = mouseSpeed * float(ysize / 2 - ypos);
			if (radius <= 1.11) {
				radius = 1.11;
			}
			else if (zoom > 0) {
				radius = ((radius - 1.0) * (1 + abs(zoom))) + 1.0;
			}
			else if (zoom < 0) {
				radius = ((radius - 1.0) / (1 + abs(zoom))) + 1.0;
			}

		}

	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		active = 1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	position = objectPos - glm::vec3((radius * cosf(verticalAngle) * sin(horizontalAngle)),
		(radius * sinf(verticalAngle)),
		(radius * cosf(verticalAngle) * cos(horizontalAngle)));

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
	//0.182327     -0.528131       0.829358 
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), (float)xsize / ysize, 0.1f, 100.0f);
	// Camera matrix
	//std::cout << position[0] << "\t" << position[1] << "\t" << position[2] << "\n";position(glm::vec3(-2.5, 3.1, -2.5)),
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

