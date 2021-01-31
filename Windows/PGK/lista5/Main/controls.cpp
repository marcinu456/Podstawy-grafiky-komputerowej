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

float Controls::getTime() {
	return this->dt;
}

glm::mat4 Controls::getViewMatrix() {
	return this->ViewMatrix;
}
glm::mat4 Controls::getProjectionMatrix() {
	return this->ProjectionMatrix;
}


void Controls::computeMatricesFromInputs() {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	this->dt = deltaTime;

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	int xsize, ysize;
	glfwGetWindowSize(window, &xsize, &ysize);

	if (active) {
		// Reset mouse position for next frame
		glfwSetCursorPos(window, xsize / 2, ysize / 2);

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(xsize / 2 - xpos);
		verticalAngle += mouseSpeed * float(ysize / 2 - ypos);
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

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 directionup(
		cos(0) * sin(0),
		sin(0),
		cos(0) * cos(0)
	);

	// Right vector
	glm::vec3 rightup = glm::vec3(
		sin(0 - 3.1416f / 2.0f),
		0,
		cos(0 - 3.1416f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(rightup, directionup);

	if (active) {
		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			this->position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			this->position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			this->position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			this->position -= right * deltaTime * speed;
			std::cout << "dop\n";
		}
		// up
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			this->position += up * deltaTime * speed;
		}
		// down
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			this->position -= up * deltaTime * speed;
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			active = 0;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		active = 1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
	glm::vec3  p = glm::vec3(-0.3, 1.5, -3.8);
	glm::vec3  d = glm::vec3(0.0, -.4, 1.0);
	//0.182327     -0.528131       0.829358 
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), (float)xsize / ysize, 0.1f, 100.0f);
	// Camera matrix
	//std::cout << position[0] << "\t" << position[1] << "\t" << position[2] << "\n";position(glm::vec3(-2.5, 3.1, -2.5)),
	ViewMatrix = glm::lookAt(
		p,           // Camera is here
		p + d, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


void Controls::computeMatricesFromInputs(glm::vec3& objectPos, bool bMcam) {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	this->dt = deltaTime;

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	int xsize, ysize;
	glfwGetWindowSize(window, &xsize, &ysize);

	if (active) {
		// Reset mouse position for next frame
		glfwSetCursorPos(window, xsize / 2, xsize / 2);

		// Compute new orientation
		horizontalAngle += mouseSpeed * float(xsize / 2 - xpos);
		verticalAngle += mouseSpeed * float(xsize / 2 - ypos);
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

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 directionup(
		cos(0) * sin(0),
		sin(0),
		cos(0) * cos(0)
	);

	// Right vector
	glm::vec3 rightup = glm::vec3(
		sin(0 - 3.1416f / 2.0f),
		0,
		cos(0 - 3.1416f / 2.0f)
	);


	// Up vector
	glm::vec3 up = glm::cross(rightup, directionup);

	if (active && bMcam) {
		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			objectPos += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			objectPos -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			objectPos += right * deltaTime * speed;
			//std::cout << "pos\n";
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			objectPos -= right * deltaTime * speed;
		}

		// up
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			objectPos += up * deltaTime * speed;
		}
		// down
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			objectPos -= up * deltaTime * speed;
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			active = 0;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	else if (active && !bMcam) {
		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			objectPos += directionup * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			objectPos -= directionup * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			objectPos += rightup * deltaTime * speed;
			//std::cout << "pos\n";
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			objectPos -= rightup * deltaTime * speed;
		}

		// up
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			objectPos += up * deltaTime * speed;
		}
		// down
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			objectPos -= up * deltaTime * speed;
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			active = 0;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		active = 1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	this->position = objectPos - glm::vec3((this->radius * cosf(verticalAngle) * sin(horizontalAngle)),
		(this->radius * sinf(verticalAngle)),
		(this->radius * cosf(verticalAngle) * cos(horizontalAngle)));

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), (float)xsize / ysize, 0.1f, 100.0f);
	// Camera matrix
	//std::cout<<"pos" << position[0] << "\t" << position[1] << "\t" << position[2] << "\n";
	//std::cout<<"dit" << direction[0] << "\t" << direction[1] << "\t" << direction[2] << "\n";
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}