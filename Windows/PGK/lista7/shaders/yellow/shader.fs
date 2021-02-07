#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D myTextureSampler;

void main(){
    vec3 vcol_prim = vec3(255,215,0);
    vec3 ocol = texture( myTextureSampler, UV ).rgb;
    ocol = ( (vcol_prim)*length((vcol_prim)) + ocol*length(ocol) * (1.0 - length((vcol_prim))) ) /4;
    color = vec4(ocol,1);
}