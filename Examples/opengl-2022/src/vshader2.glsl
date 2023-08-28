#version 150

in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform vec3 theta;
uniform mat4 ModelView, Projection;

void main() 
{    
    color = vColor;
    gl_Position = Projection * ModelView * vPosition;
} 
