#ifndef TERRAIN_INCLUDED
#define TERRAIN_INCLUDED
#define _USE_MATH_DEFINES
#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"

#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <future>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include "FileLoader.hpp"

class Area : public AGLDrawable {
public:
    glm::vec3 pos;
    glm::vec3 scale;


    Area();
    ~Area();
    void setShaders();
    void setBuffers();
    void draw(glm::mat4& MVP, glm::vec3& light);
    void refresh(glm::vec3 campos, int _var = 500);
private:
    static std::vector<GLfloat> generateMesh(float verticalDeg, float horizontalDeg, float radius, int _var);
    bool loadBuff;

    std::vector<GLfloat> g_vertex_buffer_data;
    const GLfloat* g_color_buffer_data;
    GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };

    GLuint elementbuffer;


    std::thread thread;
    void threadLoop();
    bool endThread = false;
    std::mutex endThread_mux;
    glm::vec3 curCamPos;
    std::mutex curCamPos_mux;
    std::mutex loadBuff_mux;
    int curvar;

};

#endif

