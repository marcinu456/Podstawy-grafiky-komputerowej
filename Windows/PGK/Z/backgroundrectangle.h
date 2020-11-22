#ifndef BACKGROUNDRECTANGLE_H
#define BACKGROUNDRECTANGLE_H
#include <array>
#include "AGL3Drawable.hpp"
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>

class BackgroundRectangle : public AGLDrawable
{
public:
    BackgroundRectangle(GLfloat xStart, GLfloat yStart, GLfloat xEnd, GLfloat yEnd);
    void setShaders();
    void setBuffers();
    void draw(float tx, float ty, float scale, glm::mat4 view);
private:
    std::array<std::array<GLfloat,2>, 6> vert;
};

#endif // BACKGROUNDRECTANGLE_H
