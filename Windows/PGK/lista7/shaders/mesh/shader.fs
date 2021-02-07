#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D myTextureSampler;

void main(){
    vec3 ocol = texture( myTextureSampler, UV ).rgb;
    color = vec4(ocol,1);
}