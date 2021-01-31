#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <cstdio>
#include <ctime>


#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"
#include "Shapes.hpp"

#include "controls.hpp"
#include <glm/glm.hpp>






class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void setup();
    static void reshape(GLFWwindow * a, int b,int c);
    void MainLoop(long int grid_size, bool is_seed, int SEED);
};


void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
        ; // do something
    }
    if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
        ; // do something
    }
}

void MyWin::setup(){

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


void MyWin::MainLoop(long int grid_size,bool is_seed,int SEED) {

    int wins = 0;
    
    std::clock_t start = clock();

    //glfwSetWindowSizeCallback(win(), MyWin::CallbackResize);
    this->setup();

    Controls camera(win());
    camera.radius = 5.0f/grid_size;
    camera.speed = 5.0f/grid_size;
    bool switchCamera = 0;
    bool vievMainCamera = 1;

    Controls camera2(win());


    Cube cube;
    cube.scale[2] = 2;
    //cube.setColor(1, 1, 1 );
    SphereBubbles* obsticles;// (grid_size, is_seed, SEED);
    obsticles = new SphereBubbles(grid_size);

    Sphere player(100);
    player.scale[0] = 0.5 / grid_size;
    player.scale[1] = 0.5 / grid_size;
    player.scale[2] = 0.5 / grid_size;
    player.pos[0] = (1.0f/grid_size)-1;
    player.pos[1] = (1.0f/grid_size)-1;
    player.pos[2] = (1.0f/grid_size)-1;
    int lives = 3;
    float lives_color[3] = { 1,.5,0 };

    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;
    camera2.computeMatricesFromInputs(); 
    int xsize, ysize;
	glfwGetWindowSize(win(), &xsize, &ysize);


    player.pos[2] = -1.5;
    do {
        glfwGetWindowSize(win(), &xsize, &ysize);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //allows moving in other view
        
        obsticles->move((camera.getTime()* (wins + 1)));

        if (vievMainCamera) {
            camera.computeMatricesFromInputs(player.pos,true);
            ProjectionMatrix = camera.getProjectionMatrix();
            ViewMatrix = camera.getViewMatrix();
            ModelMatrix = glm::mat4(1.0);
            MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            AGLErrors("main-loopbegin");

            cube.draw(MVP, lives_color[lives-1]);
            obsticles->draw(MVP, player.pos);
            glClear(GL_DEPTH_BUFFER_BIT);
            player.draw(MVP);
        }
        else
        {
            camera.computeMatricesFromInputs(player.pos, false);
            AGLErrors("main-afterdraw");
            AGLErrors("main-loopbegin");

            ProjectionMatrix = camera2.getProjectionMatrix();
            ViewMatrix = camera2.getViewMatrix();
            ModelMatrix = glm::mat4(1);
            MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            cube.draw(MVP, lives_color[lives - 1]);
            obsticles->draw(MVP, player.pos);
            player.draw(MVP);
            AGLErrors("main-afterdraw");
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

        // Colisions
        if (player.pos[2] + player.scale[2] > cube.scale[2]) {
            player.pos[2] = -1.5;
            wins++;
        }
        if (abs(player.pos[0]) + player.scale[0] > cube.scale[0]) player.pos[0] = (player.pos[0] > 0 ? cube.scale[0] - player.scale[0] : -cube.scale[0] + player.scale[0]);
        if (abs(player.pos[1]) + player.scale[1] > cube.scale[1]) player.pos[1] = (player.pos[1] > 0 ? cube.scale[1] - player.scale[1] : -cube.scale[1] + player.scale[1]);
        if (abs(player.pos[2]) + player.scale[2] > cube.scale[2]) player.pos[2] = (player.pos[2] > 0 ? cube.scale[2] - player.scale[2] : -cube.scale[2] + player.scale[2]);

        for (size_t i = 0; i < obsticles->sizeInstances; i++) {
            if (
                abs(player.pos[0] - obsticles->pos[i][0]) < player.scale[0] + obsticles->scale[i][0] &&
                abs(player.pos[1] - obsticles->pos[i][1]) < player.scale[1] + obsticles->scale[i][1] &&
                abs(player.pos[2] - obsticles->pos[i][2]) < player.scale[2] + obsticles->scale[i][2]
                )
            {
                player.pos[2] = -1.5;
                if (lives != 0)
                {
                    lives--;
                    std::cout << lives << "\n";
                }

            }
        }

        if (lives == 0)
            break;

    } // end of do
    while (glfwWindowShouldClose(win()) == 0 || camera.active || lives == 0);

    std::cout << "You end levels "<<wins <<" times";
}


int main(int argc, char *argv[]) {

    bool is_seed = false;
    int SEED = 10;

    int N_size = 10;
    if (argc == 2)
    {
        SEED = int(atoi(argv[1]));
        is_seed = true;
    }
    else if (argc == 3)
    {
        SEED = int(atoi(argv[1]));
        N_size = int(atoi(argv[2]));
        is_seed = true;
    }

    std::cout << "Current seed: " << SEED << "\n";
    std::cout << "Current N: " << N_size << "\n";

    MyWin win;
    win.Init(800,800,"Labirynt 3D",0,33);

    time_t start = time(0);

    win.MainLoop(N_size, is_seed,SEED);

    std::cout << "Time played: " << difftime(time(0), start) << "s\n";

    return 0;
}