#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED
#define _USE_MATH_DEFINES
#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"

#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


class Sphere : public AGLDrawable {
public:
    glm::vec3 pos;
    glm::vec3 scale;


    Sphere(int N);
    Sphere();
    void setShaders();
    void setBuffers();
    void draw(glm::mat4& MVP);

 private:
    static std::vector<GLfloat> generateMesh(int N);
    
    const std::vector<GLfloat> g_vertex_buffer_data;
    const GLfloat* g_color_buffer_data;

};

#endif