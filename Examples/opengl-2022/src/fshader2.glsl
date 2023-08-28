#version 150

in  vec4 color;
out vec4 fColor;

uniform vec3 theta;

void main() 
{ 
    //fColor = color * sin(radians(theta.x));
    //fColor = color * abs(1 - theta.x/180.0);
    fColor = color;
}
