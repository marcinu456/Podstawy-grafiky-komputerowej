#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <thread>

#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"
#include "Shapes.hpp"

#include "controls.hpp"
#include <glm/glm.hpp>






class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char* name, int vers, int fullscr = 0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void setup();
    static void reshape(GLFWwindow* a, int b, int c);
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

    this->setup();
    bool refrsh = 1;
    bool test = true;
    Controls camera(win());
    camera.radius = 1.12;
    camera.speed = 5.0f;
    camera.setCoord(0, -80);
    bool switchCamera = 0;

    Sphere water(30,true);
    water.setColor(0, 0, 1);
    Sphere planet(30,false);
    planet.scale *= 1.003;
    Area area;


    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;


    int xsize, ysize;
    glfwGetWindowSize(win(), &xsize, &ysize);
    bool er = false;
    do {
        glfwGetWindowSize(win(), &xsize, &ysize);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //allows moving in other view



        camera.computeMatricesFromInputs(planet.pos);
        ProjectionMatrix = camera.getProjectionMatrix();
        ViewMatrix = camera.getViewMatrix();
        ModelMatrix = glm::mat4(1.0);
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        //area.refresh(camera.position);
        AGLErrors("main-loopbegin");


        if (glfwGetMouseButton(win(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            if(refrsh == 1)
            area.refresh(camera.position);
        }


        area.draw(MVP, camera.position);
        water.draw(MVP, camera.position);
        if (switchCamera ==1) {
            planet.draw(MVP, camera.position);
            
        }


        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();




        if (glfwGetKey(win(), GLFW_KEY_TAB) == GLFW_PRESS) {
            switchCamera = 1;
        }
        if (switchCamera == 1 && glfwGetKey(win(), GLFW_KEY_TAB) == GLFW_RELEASE) {
            switchCamera = 0;

        }

        if (glfwGetKey(win(), GLFW_KEY_0) == GLFW_PRESS) {
            camera.radius = 3;

            std::cout << " press 0'\n";
        }
        if (glfwGetKey(win(), GLFW_KEY_1) == GLFW_PRESS) {
            camera.radius = 2.5;
            area.refresh(camera.position);
        }
        if (glfwGetKey(win(), GLFW_KEY_2) == GLFW_PRESS) {
            camera.radius = 2;
            area.refresh(camera.position);
        }
        if (glfwGetKey(win(), GLFW_KEY_3) == GLFW_PRESS) {
            camera.radius = 1.5;
            area.refresh(camera.position);
        }
        if (glfwGetKey(win(), GLFW_KEY_4) == GLFW_PRESS) {
            camera.radius = 1.25;
            area.refresh(camera.position);
        }
        if (glfwGetKey(win(), GLFW_KEY_5) == GLFW_PRESS) {
            camera.radius = 1.15;
            area.refresh(camera.position);
        }
        if (glfwGetKey(win(), GLFW_KEY_6) == GLFW_PRESS) {
            camera.radius = 1.12;
            std::cout << camera.radius << "cameta\n";
            std::cout << glm::length(camera.position) << "pos\n";
            area.refresh(camera.position);
        }
        if (glfwGetKey(win(), GLFW_KEY_7) == GLFW_PRESS) {
            float cor = camera.radius*1;
            camera.radius = 1.12;
            std::cout << camera.radius << "cameta\n";
            std::cout << glm::length(camera.position) << "pos\n";
            area.refresh(camera.position,2000);
        }
        if (glfwGetKey(win(), GLFW_KEY_R) == GLFW_PRESS) {
                refrsh = 1;
            }
        if (glfwGetKey(win(), GLFW_KEY_T) == GLFW_PRESS) {
                refrsh = 0;
        }
    } // end of do
    while (glfwWindowShouldClose(win()) == 0 || camera.active);

}


int main(int argc, char* argv[]) {


    MyWin win;
    win.Init(800, 800, "Labirynt 3D", 0, 33);



    win.MainLoop();

    return 0;
}