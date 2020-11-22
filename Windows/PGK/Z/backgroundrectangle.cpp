#include "backgroundrectangle.h"



BackgroundRectangle::BackgroundRectangle(GLfloat xStart, GLfloat yStart, GLfloat xEnd, GLfloat yEnd)
{
    vert[0][0] = xStart;
    vert[0][1] = yStart;
    vert[1][0] = xEnd;
    vert[1][1] = yStart;
    vert[2][0] = xStart;
    vert[2][1] = yEnd;
    vert[3][0] = xEnd;
    vert[3][1] = yEnd;
    vert[4][0] = xStart;
    vert[4][1] = yEnd;
    vert[5][0] = xEnd;
    vert[5][1] = yStart;

    setShaders();
    setBuffers();
}
void BackgroundRectangle::setShaders()
{
    compileShaders(R"END(

                   #version 330
                   #extension GL_ARB_explicit_uniform_location : require
                   #extension GL_ARB_shading_language_420pack : require
                   layout(location = 0) in vec2 pos;
                   layout(location = 0) uniform float scale;
                   layout(location = 1) uniform vec2  center;
                   layout(location = 2) uniform vec3  first_color;
                   layout(location = 3) uniform vec3  second_color;
                   layout(location = 4) uniform mat4  view;
                   out vec2 pvec;
                   flat out vec3 color1;
                   flat out vec3 color2;
                   void main(void) {
                       vec2 p = (pos * scale + center);
                       gl_Position = vec4(p, 0.0, 1.0)*view;
                       color1 = first_color;
                       color2 = second_color;
                       pvec = pos;
                   }

                   )END", R"END(

                          #version 330
                          flat in vec3 color1;
                          flat in vec3  color2;
                          in vec2 pvec;


                          void main(void) {
                                if(pvec.x > -1 && pvec.x < -0.8 || pvec.x > -0.6 && pvec.x < -0.4 || pvec.x > -0.2 && pvec.x < 0 || pvec.x > 0.2 && pvec.x < 0.4 || pvec.x > 0.6 && pvec.x < 0.8)
                                {
                                    gl_FragColor = vec4(color1, 1);
                                }
                                else
                                {
                                    gl_FragColor = vec4(color2, 1);
                                }

                          }

                          )END");
                          }



void BackgroundRectangle::setBuffers()
{
    bindBuffers();
    GLfloat temp[6][2] = {{vert[0][0], vert[0][1]}, {vert[1][0], vert[1][1]}, {vert[2][0], vert[2][1]}, {vert[3][0], vert[3][1]}, {vert[4][0], vert[4][1]}, {vert[5][0], vert[5][1]}};
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), temp, GL_STATIC_DRAW);
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

void BackgroundRectangle::draw(float tx, float ty, float scale, glm::mat4 view)
{
    bindProgram();
    bindBuffers();
    glUniformMatrix4fv(4, 1, GL_FALSE, &view[0][0]);
    glUniform1f(0, scale);  // scale  in vertex shader
    glUniform2f(1, tx, ty);  // center in vertex shader
    glUniform3f(2, 1.0, 1, 0);
    glUniform3f(3, 0.5, 0.8, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
