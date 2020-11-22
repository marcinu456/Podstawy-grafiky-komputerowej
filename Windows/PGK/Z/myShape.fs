#version 330
in vec3 vColor;



void main(void)
{
    gl_FragColor = vec4(vColor,1);

}
