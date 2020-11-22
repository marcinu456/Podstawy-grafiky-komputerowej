#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec3 aPos;
layout(location = 1) in mat4 model;
layout(location = 5) in float scale;
layout(location = 0) uniform mat4  view;
layout(location = 1) uniform mat4  projection;
out vec3 vColor;

void main(void)
	{
            vec4 temp = model * vec4(aPos * 0.1, 1.0f);
            float len = length(temp);
            vColor = vec3(abs(temp.x / len), abs(temp.y / len), abs(temp.z / len));
            gl_Position =  projection * view * model * vec4(aPos * scale, 1.0f);

        }

