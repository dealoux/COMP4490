#version 150

in vec4 vPosition;
uniform mat4 ModelView, Projection;
uniform vec4 Color;
uniform float T;
out vec4 vColor;

void main()
{
  vColor = Color;
  vec4 eyeCoord = ModelView * vPosition;
  eyeCoord.y = eyeCoord.y * (1+T/5);
  gl_Position = Projection * eyeCoord;
}

