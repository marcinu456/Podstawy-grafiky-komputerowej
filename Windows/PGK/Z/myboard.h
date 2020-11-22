#ifndef MYBOARD_H
#define MYBOARD_H
#define _USE_MATH_DEFINES
#include <vector>
#include <array>
#include <random>
#include <memory>
#include <iomanip>
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
#include "myline.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>

class MyBoard
{
public:
    MyBoard(GLuint dimensions, unsigned int xIndexPlayer, unsigned int yIndexPlayer);
    MyBoard(GLuint dimensions, unsigned int xIndexPlayer, unsigned int yIndexPlayer, int seed);
    void setVertices(GLfloat kLineGap, GLfloat windowSize);
    void drawAll(GLfloat xWindowCenter, GLfloat yWindowCenter, GLfloat scale, glm::mat4 matrix);
    std::array<int, 2> pushLine(int unsigned xIndex, unsigned int yIndex, GLfloat speed);
    std::array<int, 2> rotateLine(unsigned int xIndex, unsigned int yIndex, GLfloat angle);
    std::array<int, 2> detectCollision(GLuint xIndex, GLuint yIndex);
    void setPlayerColor(unsigned int xIndexPlayer, unsigned int yIndexPlayer, GLfloat r1, GLfloat g1, GLfloat b1, GLfloat r2, GLfloat g2, GLfloat b2);
private:
    GLfloat vectorProduct(GLfloat xA, GLfloat yA, GLfloat xB, GLfloat yB, GLfloat xC, GLfloat yC);
    GLfloat randomAngle();
    std::vector<std::vector<std::unique_ptr<MyLine>>> lineTable;
    std::array<std::array<GLfloat, 2>, 2>  rotateVertices(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat angle);
    std::array<std::array<GLfloat, 2>, 2>  rotateVertices(const std::array<std::array<GLfloat, 2>, 2>& array, GLfloat angle);
    std::mt19937 engine;
    std::uniform_real_distribution<float> dist{0, static_cast<float>(3.14f)};
    const GLuint kDimensions;
    unsigned int xIndexPlayer;
    unsigned int yIndexPlayer;
    bool isCollision (const std::array<std::array<GLfloat, 2>, 2>& ref1,  const std::array<std::array<GLfloat, 2>, 2>& ref2);
};

#endif // MYBOARD_H
