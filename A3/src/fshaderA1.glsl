#version 150

in vec4 vColor;
uniform float T;
out vec4 color;

void main() 
{
/*  color = vec4(0.2,1.0,0.5,1);*/
  color = vec4(vColor.xyz * T, 1);
}
