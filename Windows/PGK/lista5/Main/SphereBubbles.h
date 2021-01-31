#pragma once

#define _USE_MATH_DEFINES
#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"

#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <climits>
#include <ctime>

class SphereBubbles : public AGLDrawable
{
public:
    glm::vec3* pos;
    glm::vec3* scale;
    glm::vec3* rotations; // change to normals

    SphereBubbles(const size_t& N);
    ~SphereBubbles();
    void setShaders();
    void setBuffers();
    void draw(glm::mat4& MVP, const glm::vec3& light);
    void setColor(float r, float g, float b);
    void move(float dt);
    size_t sizeInstances;
    size_t N;
private:
    static std::vector<GLfloat> generateMesh(int N);
    static std::vector<GLfloat> generateNormals(int N, std::vector<GLfloat> points);

    const std::vector<GLfloat> g_vertex_buffer_data;
    const std::vector<GLfloat> g_normal_buffer_data;

    GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
   
};

