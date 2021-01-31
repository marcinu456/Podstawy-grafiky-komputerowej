#version 330

in vec3 vColor;

out vec4 vFragColor;

void main()
{
        vFragColor = vec4(vColor, 1);
}
