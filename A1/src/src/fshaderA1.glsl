#version 150

flat in vec4 f_colour;
out vec4 out_colour;

void main() 
{
  out_colour = f_colour;
}