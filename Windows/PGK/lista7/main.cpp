#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <cstdio>
#include <ctime>
#include <chrono>


#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"
#include "Mesh.hpp"

#include "controls.hpp"
#include <glm/glm.hpp>





class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char* name, int vers, int fullscr = 0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void setup();
    void MainLoop();
};


void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key, scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
        ; // do something
    }
    if (key == GLFW_KEY_HOME && (action == GLFW_PRESS)) {
        ; // do something
    }
}



void MyWin::setup() {


    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    glfwSetInputMode(win(), GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
}

void MyWin::MainLoop() {
    std::cout << -1 / 30 << "\n";

    this->setup();

    Controls camera(win());
    camera.radius = 8;
    camera.speed = 5.0f;
    camera.setCoord(0, 0);
    bool switchCamera = 0;
    bool vievMainCamera = 1;

    Mesh mesh_0("OBJ\\dennis");
    Mesh mesh_1("OBJ\\suzanne");

    mesh_0.scale *= 0.125;
    mesh_1.scale *= 0.0025;

    mesh_0.setShader("shaders\\perlin");
    mesh_1.setShader("shaders\\mesh");


    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;


    int xsize, ysize;
    glfwGetWindowSize(win(), &xsize, &ysize);

    do {
        glfwGetWindowSize(win(), &xsize, &ysize);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //allows moving in other view
        //camera.computeMatricesFromInputs(globeBg.pos);

        if (vievMainCamera) {
            camera.computeMatricesFromInputs(mesh_1.pos);
            ProjectionMatrix = camera.getProjectionMatrix();
            ViewMatrix = camera.getViewMatrix();
            ModelMatrix = glm::mat4(1.0);
            MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            mesh_0.draw(MVP, camera.position);
            mesh_1.draw(MVP, camera.position);

            AGLErrors("main-loopbegin");

        }
        else {
            camera.computeMatricesFromInputs();
            ProjectionMatrix = camera.getProjectionMatrix();
            ViewMatrix = camera.getViewMatrix();
            ModelMatrix = glm::mat4(1.0);
            MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            mesh_0.draw(MVP, camera.position);
            mesh_1.draw(MVP, camera.position);

            AGLErrors("main-loopbegin");

        }

        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();

        if (glfwGetKey(win(), GLFW_KEY_TAB) == GLFW_PRESS) {
            switchCamera = 1;
        }
        if (switchCamera == 1 && glfwGetKey(win(), GLFW_KEY_TAB) == GLFW_RELEASE) {
            vievMainCamera = !vievMainCamera;
            switchCamera = 0;

        }
    } // end of do
    while (glfwWindowShouldClose(win()) == 0 || (camera.active));

}


int main(int argc, char* argv[]) {


    MyWin win;
    win.Init(800, 800, "Labirynt 3D", 0, 33);



    win.MainLoop();

    return 0;
}