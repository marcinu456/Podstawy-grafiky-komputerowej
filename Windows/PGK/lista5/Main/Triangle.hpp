#ifndef TRIANGLE_INSTANCES_INCLUDED
#define TRIANGLE_INSTANCES_INCLUDED
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

class Triangle : public AGLDrawable {
public:
    glm::vec3* pos;
    glm::vec3* scale;
    glm::vec3* rotations;
    

    Triangle(const size_t& N,bool is_seed, int seed);
    void setShaders();
    void setBuffers();
    void draw(glm::mat4& MVP, double time);

    size_t sizeInstances;
 private:
    static const GLfloat g_vertex_buffer_data[];
    

};

#endif