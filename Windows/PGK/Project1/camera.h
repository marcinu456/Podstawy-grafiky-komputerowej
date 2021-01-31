#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include "AGL3Window.hpp"
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/string_cast.hpp>
class Camera
{
public:
    Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 up);
    void forwardBackwardMove(float speed);
    void rotateCamera(float pitch, float yaw);
    void updateView();
    void changeProjection(float yOffset);
    void pushCamera(glm::vec3 direction, float scale);
    glm::mat4 getProjection(float screenWidth, float screenHeight);
    glm::mat4 getView() const;
    glm::vec3 getCameraPosition() const;
    glm::vec3 getCameraTarget();


    void setCameraTarget(glm::vec3 position);
private:
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 right;
    float phi;
    float theta;
    float fov;
    float rotateRadius;

};

#endif // CAMERA_H
