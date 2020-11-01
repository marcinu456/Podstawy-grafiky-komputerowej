// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <climits>
//#include <cmath>


#include "AGL3Drawable.hpp"
#include "AGL3Window.hpp"



// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
class MyTri : public AGLDrawable {
public:
	MyTri() : AGLDrawable(0) {
		setShaders();
	}
	void setShaders() {
		compileShaders(R"END(

         #version 330 
         out vec4 vcolor;
		 out  vec2 vpos;
         void main(void) {
            const vec2 vertices[3] = vec2[3](vec2( 0.9, -0.9),
                                             vec2(-0.9, -0.9),
                                             vec2( 0.9,  0.9));
            const vec4 colors[]    = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
                                             vec4(0.0, 1.0, 0.0, 1.0),
                                             vec4(0.0, 0.0, 1.0, 1.0));

            vcolor      = colors[gl_VertexID];
			vpos = vertices[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END", R"END(

         #version 330 
		  in  vec2 vpos;
         in  vec4 vcolor;
         out vec4 color;

         void main(void) {
          float r = vpos.x * vpos.x + vpos.y * vpos.y;
           if (r < 0.01) {
              color = vec4(1.0, 1.0, 1.0, 0);
            }
            else {
              color = vcolor;
           }
         } 

      )END");
	}
	void draw() {
		bindProgram();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	}
};


// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment programs
// ==========================================================================
class Lines : public AGLDrawable {
public:
	float rotate;
	float x;
	float y;
	float scale;
	float color_begin[4];
	float color_end[4];

	Lines(float rotate, float x ,float y, float scale	, float RB, float GB, float BB, float RE, float GE, float BE, float Alpha ) : AGLDrawable(0),
		rotate(rotate), x(x), y(y), scale(scale), color_begin{ RB ,GB,BB,Alpha }, color_end{RE,GE,BE,Alpha}  {
			setShaders();
	}
	Lines():Lines(0,0,0,1,1,1,1,1,1,1,1) {

	}
	void setShaders() {
		compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos;
         layout(location = 0) uniform float scale;
         layout(location = 1) uniform float  rotate;
		 layout(location = 2) uniform vec2  location;
		 layout(location = 3) uniform vec4  colorbegin;
		 layout(location = 4) uniform vec4  colorend;
         out vec4 vcolor;
         void main(void) {
            const vec2 vertices[] = vec2[2](vec2( cos(rotate), sin(rotate))*scale+location,
                                             vec2(-cos(rotate), -sin(rotate))*scale+location);
            const vec4 colors[]    = vec4[2](colorbegin,
                                             colorend);

            vcolor      = colors[gl_VertexID];

            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END", R"END(

         #version 330 

         in  vec4 vcolor;
         out vec4 color;

         void main(void) {
			color = vcolor;
         } 

      )END");
	}

	void draw() {
		bindProgram();
		bindBuffers();
		glUniform1f(0, this->scale);  // scale  in vertex shader
		glUniform1f(1, this->rotate);
		glUniform2f(2, this->x, this->y);  // center in vertex shader
		glUniform4f(3, color_begin[0], color_begin[1], color_begin[2], color_begin[3]);
		glUniform4f(4, color_end[0], color_end[1], color_end[2], color_end[3]);

		glDrawArrays(GL_LINES, 0, 2);
	}
	void setColorBegin(float R,float G,float B, float A){
		color_begin[0] = R; color_begin[1] = G; color_begin[2] = B; color_begin[3] = A;
	}
	void setColorEnd(float R, float G, float B, float A) {
		color_end[0] = R; color_end[1] = G; color_end[2] = B; color_end[3] = A;
	}


};



// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
	MyWin() {};
	MyWin(int _wd, int _ht, const char* name, int vers, int fullscr = 0)
		: AGLWindow(_wd, _ht, name, vers, fullscr) {};
	virtual void KeyCB(int key, int scancode, int action, int mods);
	void MainLoop();
};


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
	AGLWindow::KeyCB(key, scancode, action, mods); // f-key full screen switch
	if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
		; // do something
	}
	if (key == GLFW_KEY_HOME && (action == GLFW_PRESS)) {
		; // do something
	}
}



// ==========================================================================
void MyWin::MainLoop() {
	
	const float PI = 3.14159265358979323846f; // Nie wiem dlaczego, ale M_PI mi nie działa
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, INT_MAX);
	ViewportOne(0, 0, wd, ht);

	Lines lines[100];
	for (int i = 1; i < 100; i++)
	{

		float random_integer = uni(rng);
		//lines[i] = { (float)(random_integer / INT_MAX) * 2 * PI,0.5,0.3,0,0,0,0,0,0,0,0 };
		lines[i].rotate = (random_integer / INT_MAX) * 2 * PI;
		lines[i].x = (i % 10)/5.f-.9f;
		lines[i].y = (i / 10)/5.f-.9f;
		lines[i].scale = .1;
		if (i==99)//finish
		{
			lines[i].setColorBegin(1, 1, 1, 1);
			lines[i].setColorEnd(1, 0, 0, 1);
		}
		else
		{
			lines[i].setColorBegin(1, 0, 1, 1);
			lines[i].setColorEnd(0, 1, 0, 1);
		}

	}
	Lines& player = lines[0];
	player.x = -.9f;
	player.y = -.9f;
	player.scale = .1;
	player.setColorBegin(1, 1, 1, 1);
	player.setColorEnd(.5, 0, .1, 1);


	MyTri   trian;
	float   tx = 0.0, ty = 0.5, cx = 0.0, cy = 0.0;
	do {
		glClear(GL_COLOR_BUFFER_BIT);

		AGLErrors("main-loopbegin");
		// =====================================================        Drawing
		//trian.draw();

		for (int i=1;i<100;i++)
		{
			lines[i].draw();
		}
		player.draw();
		//circle.draw(cx, cy);
		AGLErrors("main-afterdraw");

		glfwSwapBuffers(win()); // =============================   Swap buffers
		glfwPollEvents();
		//glfwWaitEvents();   

		if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
			tx += 0.01;
		}
		else if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS) {
			tx -= 0.01;
		}
		else if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS) { //Start Circle moving
			player.x += 0.01 * cos(player.rotate);
			player.y += 0.01*sin(player.rotate);
		}
		else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS) {
			player.x -= 0.01 * cos(player.rotate);
			player.y -= 0.01 * sin(player.rotate);
		}
		if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS) {
			player.rotate -= 0.01;
		}
		if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS) {
			player.rotate += 0.01;
		}


	} while (glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(win()) == 0);
}

int main(int argc, char* argv[]) {
	MyWin win;
	win.Init(800, 600, "AGL3 example", 0, 33);
	win.MainLoop();
	return 0;
}
