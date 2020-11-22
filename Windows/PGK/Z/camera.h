#ifndef CAMERA_H
#define CAMERA_H
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include "AGL3Window.hpp"
class Camera
{
public:
    Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 up);
    void forwardBackwardMove(float speed);
    void rotateCamera(float pitch, float yaw);
    void updateView();
    void changeProjection(float yOffset);
    glm::mat4 getProjection(float screenWidth, float screenHeight);
    glm::mat4 getView() const;
    glm::vec3 getCameraPosition() const;
private:
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 right;
    float pitch;
    float yaw;
    float fov;

};

#endif // CAMERA_H
