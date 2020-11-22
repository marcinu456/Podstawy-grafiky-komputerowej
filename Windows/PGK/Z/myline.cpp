#include "myline.h"

MyLine::MyLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) : AGLDrawable(0)
{
    vert[0][0] = x1;
    vert[0][1] = y1;
    vert[1][0] = x2;
    vert[1][1] = y2;
    setShaders();
    setBuffers();
}

MyLine::MyLine(const std::array<std::array<GLfloat, 2>, 2>& array) : AGLDrawable(0)
{
    vert = array;
    setShaders();
    setBuffers();
}

void MyLine::setShaders()
{
    compileShaders(R"END(

                   #version 330
                   #extension GL_ARB_explicit_uniform_location : require
                   #extension GL_ARB_shading_language_420pack : require
                   layout(location = 0) in vec2 pos;
                   layout(location = 0) uniform float scale;
                   layout(location = 1) uniform vec2  center;
                   layout(location = 2) uniform vec3  cross_color1;
                   layout(location = 3) uniform vec3  cross_color2;
                   layout(location = 4) uniform mat4  view;
                   out vec4 vcolor;

                   void main(void) {
                   vec2 p = (pos * scale + center);
                   gl_Position = vec4(p, 0.0, 1.0) * view;
                   if(gl_VertexID == 0)
                   {
                        vcolor = vec4(cross_color1, 1.0);
                   }
                   else
                   {
                        vcolor = vec4(cross_color2, 1.0);
                   }
                   }

                   )END", R"END(

                          #version 330

                          in vec4 vcolor;


                          void main(void) {
                            gl_FragColor = vcolor;
                          }

                          )END");

}

void MyLine::setBuffers() {
    bindBuffers();

    GLfloat temp[2][2]= {{vert[0][0], vert[0][1]}, {vert[1][0], vert[1][1]}};
    glBufferData(GL_ARRAY_BUFFER, 2*2*sizeof(float), temp, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                0,                 // attribute 0, must match the layout in the shader.
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,//24,             // stride
                (void*)0            // array buffer offset
                );
}

void MyLine::draw(float tx, float ty, float scale, glm::mat4 matrix)
{
    bindProgram();
    bindBuffers();
    glUniform1f(0, scale);  // scale  in vertex shader
    glUniform2f(1, tx, ty);  // center in vertex shader
    glUniform3f(2, face_color[0], face_color[1], face_color[2]);
    glUniform3f(3, back_color[0], back_color[1], back_color[2]);
    glUniformMatrix4fv(4, 1, GL_FALSE, &matrix[0][0]);
    glDrawArrays(GL_LINES, 0, 2);
}

void MyLine::setFaceColor(float r, float g, float b)
{
    face_color[0]=r;
    face_color[1]=g;
    face_color[2]=b;
}

void MyLine::setBackColor(float r, float g, float b)
{
    back_color[0]=r;
    back_color[1]=g;
    back_color[2]=b;
}

void MyLine::setPosition(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    vert[0][0] = x1;
    vert[0][1] = y1;
    vert[1][0] = x2;
    vert[1][1] = y2;
    //this->draw(0,0,1);
    setBuffers();
}

void MyLine::setPosition(const std::array<std::array<GLfloat, 2>, 2>& array)
{
    vert = array;
    setBuffers();
}

std::array<std::array<GLfloat, 2>, 2> MyLine::getVertices()
{
    return vert;
}
