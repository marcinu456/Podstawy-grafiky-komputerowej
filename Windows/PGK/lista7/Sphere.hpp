#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED
#define _USE_MATH_DEFINES
#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"

#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "Area.hpp"

class Sphere : public AGLDrawable {
public:
    glm::vec3 pos;
    glm::vec3 scale;


    Sphere(int N, bool planet);
    Sphere();
    void setShaders();
    void setBuffers();
    void draw(const glm::mat4& MVP, const glm::vec3& cam);
    void setColor(float r, float g, float b);
private:
    static std::vector<GLfloat> generateMesh(int N);
    static std::vector<GLfloat> generateNormals(std::vector<GLfloat> points);

    bool planet;
    const std::vector<GLfloat> g_vertex_buffer_data;
    const std::vector<GLfloat> g_normal_buffer_data;
    const GLfloat* g_color_buffer_data;
    GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
};

#endif