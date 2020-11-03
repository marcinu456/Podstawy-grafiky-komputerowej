﻿// ==========================================================================
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
#include <ctime>
//#include <cmath>


#include "AGL3Drawable.hpp"
#include "AGL3Window.hpp"


float iloczyn_wektorowy(std::vector<float> X, std::vector<float> Y, std::vector<float> Z)
{
	float x1 = Z[0] - X[0], y1 = Z[1] - X[1],
		x2 = Y[0] - X[0], y2 = Y[1] - X[1];
	return x1 * y2 - x2 * y1;
}
//sprawdzenie, czy punkt Z(koniec odcinka pierwszego)
//leży na odcinku XY
bool sprawdz(std::vector<float> X, std::vector<float> Y, std::vector<float> Z)
{
	return std::min(X[0], Y[0]) <= Z[0] && Z[0] <= std::max(X[0], Y[0])
		&& std::min(X[1], Y[1]) <= Z[1] && Z[1] <= std::max(X[1], Y[1]);
}

bool czy_przecinaja(std::vector<float> A, std::vector<float> B, std::vector<float> C, std::vector<float> D)
{
	float v1 = iloczyn_wektorowy(C, D, A),
		v2 = iloczyn_wektorowy(C, D, B),
		v3 = iloczyn_wektorowy(A, B, C),
		v4 = iloczyn_wektorowy(A, B, D);

	//sprawdzenie czy się przecinają - dla niedużych liczb
	if(v1*v2 < 0 && v3*v4 < 0) return 1;

	//sprawdzenie czy się przecinają - dla większych liczb
	if ((v1 > 0 && v2 < 0 || v1 < 0 && v2>0) && (v3 > 0 && v4 < 0 || v3 < 0 && v4>0)) return 1;

	//odcinki nie mają punktów wspólnych
	return 0;
}



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
            const vec2 vertices[] = vec2[4](vec2( 0.9, -0.9),
                                             vec2(-0.9, -0.9),
 vec2( -0.9,  0.9),
                                             vec2( 0.9,  0.9)
											);
            const vec4 colors[]    = vec4[4](vec4(1.0, 0.0, 0.0, 1.0),
                                             vec4(0.0, 1.0, 0.0, 1.0),
                                             vec4(0.0, 0.0, 1.0, 1.0),
											 vec4(0.0, 0.5, 1.0, 1.0));

            vcolor      = colors[gl_VertexID];
			vpos = vertices[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END", R"END(

         #version 330 
		precision highp float;
		in vec2 vpos;
		in vec4 vcolor;
		out vec4 color;

		float pulse(float val, float dst) {
		  return floor(mod(val*dst,1.0)+.5);
		}

		void main()
		{
		  vec2 dir = vec2(0,10); // high noon
  
		  //vec2 cpos = vpos;
  
		  const float d = 5.0;

		  float bright = pulse(vpos.x,d) + pulse(vpos.y,d);

		  vec3 vvcolor = mod(bright,2.0) > .5 ? vec3(1,1,1) : vec3(0,0.1,0.1); 

		  float diffuse = .5 + dot(vec2(1,0),dir);
		  color = vec4(diffuse * vvcolor, 1);
		//color=vcolor;
		}


      )END");
	}
	void draw(std::clock_t clock) {
		bindProgram();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
	std::vector<float> odcinek_begin;
	std::vector<float> odcinek_end;

	Lines(float rotate, float x ,float y, float scale	, float RB, float GB, float BB, float RE, float GE, float BE, float Alpha ) : AGLDrawable(0),
		rotate(rotate), x(x), y(y), scale(scale), color_begin{ RB ,GB,BB,Alpha }, color_end{ RE,GE,BE,Alpha } 
		//odcinek_begin{(cos(rotate) * scale + x), (sin(rotate) * scale + y)  },
		//odcinek_end{(-cos(rotate) * scale + x), (-sin(rotate) * scale + y)  }  
	{
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
		odcinek_begin.push_back((cos(this->rotate) * this->scale + this->x));
		odcinek_begin.push_back((sin(this->rotate) * this->scale + this->y));
		odcinek_end.push_back((-cos(this->rotate) * this->scale + this->x));
		odcinek_end.push_back((-sin(this->rotate) * this->scale + this->y));
	}
	void setColorEnd(float R, float G, float B, float A) {
		color_end[0] = R; color_end[1] = G; color_end[2] = B; color_end[3] = A;
	}

	void updateplayercollision() {
		odcinek_begin[0] = (cos(this->rotate) * this->scale + this->x);
		odcinek_begin[1] = (sin(this->rotate) * this->scale + this->y);
		odcinek_end[0] = (-cos(this->rotate) * this->scale + this->x);
		odcinek_end[1] = (-sin(this->rotate) * this->scale + this->y);
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
	void MainLoop(int argc, char* argv[]);


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
void MyWin::MainLoop(int argc, char* argv[]) {

	std::clock_t start;
	double duration;
	start = std::clock();

	const float PI = 3.14159265358979323846f; // Nie wiem dlaczego, ale M_PI mi nie działa
	std::random_device rd;
	
	std::mt19937 rng;
	int liczba_N = 0;
	if (argc < 2)
	{
		std::mt19937 rpg(rd());
		rng = rpg;
	}
	if (argc == 2)
	{
		std::mt19937 rpg((char)argv[1]);
		rng = rpg;
	}
	else
	{
		std::mt19937 rpg((char)argv[1]);
		rng = rpg;
		liczba_N = atoi(argv[2]);
	}
	std::uniform_int_distribution<int> uni(0, INT_MAX);
	ViewportOne(0, 0, wd, ht);
	const int N = 10;
	Lines lines[N * N];
	for (int i = 1; i < N * N; i++)
	{

		float random_integer = uni(rng);
		lines[i].rotate = (random_integer / INT_MAX) * 2 * PI;
		lines[i].x = (i % N)/(N/2.f)-.9f;
		lines[i].y = (i / N)/(N / 2.f)-.9f;
		lines[i].scale = 1.f/N;
		if (i== (N * N)-1)//finish naprawic
		{
			lines[i].setColorBegin(1, 1, 1, 1);
			lines[i].setColorEnd(1, 0, 0, 1);
		}
		else
		{
			lines[i].setColorBegin(1, 0, 1, 1);
			lines[i].setColorEnd(0, 1, 0, 1);
		}

		//std::cout << i << " begin " << lines[i].odcinek_begin[0]<< "  " << lines[i].odcinek_begin[1] << "\n";
		//std::cout << i << " end " << lines[i].odcinek_end[0]<< "   " << lines[i].odcinek_end[1] << "\n";
	}
	Lines& player = lines[0];
	player.x = -.9f;
	player.y = -.9f;
	player.scale = .1;
	player.setColorBegin(0, .1, 1, 1);
	player.setColorEnd(.5, 1, .1, 1);
	std::cout <<  " player begin " << player.odcinek_begin[0] << "  " << player.odcinek_begin[1] << "\n";
	std::cout <<  " player end " << player.odcinek_end[0] << "   " << player.odcinek_end[1] << "\n";


	bool test = false;
	MyTri   trian;
	float   tx = 0.0, ty = 0.5, cx = 0.0, cy = 0.0;
	do {
		glClear(GL_COLOR_BUFFER_BIT);

		AGLErrors("main-loopbegin");
		// =====================================================        Drawing
		trian.draw(std::clock());

		for (int i=1;i< N * N;i++)
		{
			lines[i].draw();
			if (czy_przecinaja(player.odcinek_begin, player.odcinek_end, lines[i].odcinek_begin, lines[i].odcinek_end)) {
				
				if (i == (N * N) - 1) {
					test = true;
					break;
				}	
				player.x = -.9f;
				player.y = -.9f;
				player.rotate = 0;
			}
		}
		player.draw();
		player.updateplayercollision();
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


		float scale = sqrt(1.f / 16.f) / 3;

		// collision x-axis?
		bool collisionX = cx + scale >= tx && tx + scale >= cx;
		// collision y-axis?
		bool collisionY = cy + scale >= ty && ty + scale >= cy;
		// collision only if on both axes

		//if (collisionX && collisionY) {
		//	std::cout << "colid \n";
		//	break;
		//}
		//else
		//	std::cout << "cross " << tx << " | " << ty << " circle" << cx << " " << cy << "\n";
		if (test)
		{
			std::cout << "brawo \n";
			break;
			
		}

	} while (glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(win()) == 0);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Twoj czas to: " << duration << '\n';
}

int main(int argc, char* argv[]) {
	MyWin win;
	win.Init(800, 600, "AGL3 example", 0, 33);
	//win.sizeargs = argc;
	//for (int i = 1; i < argc; i++)
	//{
	//	win.args[i] = argv[i];
	//}
	win.MainLoop(argc, argv);
	return 0;
}
