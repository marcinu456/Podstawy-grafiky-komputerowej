#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Controls{
public:
    Controls(GLFWwindow *window);
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

    GLfloat radius;

    bool active = 1;

    glm::vec3 position; 
    float horizontalAngle;
    float verticalAngle;
    float initialFoV;

    float speed; // 3 units / second
    float mouseSpeed;

    GLFWwindow *window;

    void computeMatricesFromInputs();
    void computeMatricesFromInputs(glm::vec3& object);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

};


#endif