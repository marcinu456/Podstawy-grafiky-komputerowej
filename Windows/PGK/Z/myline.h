#ifndef MYLINE_H
#define MYLINE_H
#include <iostream>
#include <array>
#include "AGL3Drawable.hpp"
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
class MyLine : public AGLDrawable
{
public:
    MyLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    MyLine(const std::array<std::array<GLfloat, 2>, 2>& array);
    void setShaders();
    void setBuffers();
    void draw(float tx, float ty, float scale, glm::mat4 matrix);
    void setFaceColor(float r, float g, float b);
    void setBackColor(float r, float g, float b);
    void setPosition(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void setPosition(const std::array<std::array<GLfloat, 2>, 2>& array);
    std::array<std::array<GLfloat, 2>, 2> getVertices();

private:
    GLfloat face_color[3] = { 1.0, 0.2, 0.0 };
    GLfloat back_color[3] = { 0.0, 0.0, 0.8 };
    std::array<std::array<GLfloat, 2>, 2> vert;
    bool player = false;
};
#endif // MYLINE_H
