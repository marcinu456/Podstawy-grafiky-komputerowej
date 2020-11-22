#ifndef MYTRIANGLE_H
#define MYTRIANGLE_H
#include <vector>
#include <array>
#include <cmath>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "AGL3Drawable.hpp"
#include "triangle.h"


#include <glm/gtx/string_cast.hpp>
class myShape : public AGLDrawable
{
public:
    myShape(unsigned int dimensions, int seed);
    void setMidpoints(float xStart, float yStart, float zStart, float dimensionSize);
    void draw(float tx, float ty, float scale, glm::mat4 view, const glm::mat4 &projection);
    bool isCollision(glm::vec3 spherePosition, float sphereRay);

private:
    void setShaders();
    void setBuffers();
    void setRotations();
    void setScales();
    void setTriangleCache();

    float vectorProduct(float xA, float yA, float xB, float yB, float xC, float yC);
    bool isIntersection(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);
    const unsigned int kDimensions;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int midpointsVBO;
    unsigned int scalesVBO;
    std::mt19937 engine;
    std::uniform_real_distribution<float> angleDist{0, 180.0f};
    std::uniform_real_distribution<float> scaleDist{1.0f, 3.0f};
    std::array<float, 9> triangleVertices = {0.0f   , 1.0f, 0.0f,
                                             0.5773f, 0.0f, 0.0f
                                            -0.5773f, 0.0f, 0.0f,
                                            };


    std::vector<Triangle> triangleCache; //objects with precomputed collision parts
    std::vector<glm::vec3> trianglesMidpoints;
    std::vector<float> trianglesAngles;
    std::vector<float> trianglesScales;
};

#endif // MYTRIANGLE_H
