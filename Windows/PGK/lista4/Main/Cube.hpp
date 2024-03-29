#ifndef CUBE_INCLUDED
#define CUBE_INCLUDED

#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"

#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


class Cube : public AGLDrawable {
public:
    double_t scale;
    double_t x;
    double_t y;

    Cube();
    void setShaders();
    void setBuffers();
    void draw(glm::mat4& MVP);
    void setColor(float r, float g, float b);
 private:
    static const GLfloat g_vertex_buffer_data[];
    static const GLfloat g_color_buffer_data[];
    
    GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
};

#endif