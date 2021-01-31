#include "camera.h"

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 up) : cameraPosition(cameraPosition), cameraTarget(cameraTarget), up(up), worldUp(up)
{
    phi  = 0.0f;
    theta = 0.0f;
    fov = 45;
    rotateRadius = 5.0f;
    rotateCamera(phi, theta);
    updateView();
    rotateCamera(phi, theta);
    updateView();
}

void Camera::forwardBackwardMove(float speed)
{
    cameraTarget += glm::normalize(cameraTarget - cameraPosition) * speed;
    cameraPosition += glm::normalize(cameraTarget - cameraPosition) * speed;
}

void Camera::rotateCamera(float tPhi, float tTheta)
{
    phi  += tPhi;
    theta += tTheta;
    if(phi > 89.0f)
    {
        phi = 89.0f;
    }
    else if(phi < -89.0f)
    {
        phi = -89.0f;
    }

    if(theta > 360.0f)
    {
        theta = theta - 360.0f;
    }
    else if(theta < 0)
    {
        theta = 360 + theta;
    }
    cameraPosition.x = cameraTarget.x + rotateRadius * glm::cos(glm::radians(phi)) * glm::cos(glm::radians(theta));
    cameraPosition.y = cameraTarget.y + rotateRadius * glm::sin(glm::radians(phi));
    cameraPosition.z = cameraTarget.z + rotateRadius * glm::cos(glm::radians(phi)) * glm::sin(glm::radians(theta));
}

void Camera::updateView()
{
    view = glm::lookAt(cameraPosition, cameraTarget, up);
}

void Camera::changeProjection(float yOffset)
{
    fov -= yOffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void Camera::pushCamera(glm::vec3 direction, float scale)
{
    cameraTarget += glm::normalize(direction) * scale;
}

glm::mat4 Camera::getProjection(float screenWidth, float screenHeight)
{
    projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, 1.0f, 100.0f);
    return projection;
}

glm::mat4 Camera::getView() const
{
    return view;
}

glm::vec3 Camera::getCameraPosition() const
{
    return cameraPosition;
}

glm::vec3 Camera::getCameraTarget()
{
    return cameraTarget;
}

void Camera::setCameraTarget(glm::vec3 position)
{
    cameraTarget = position;
}




