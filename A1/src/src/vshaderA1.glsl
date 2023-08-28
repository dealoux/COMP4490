#version 150

in vec4 vPosition;
in int VERTICES_PER_FACE; 

uniform mat4 ModelView, Projection;

flat out vec4 f_colour;

void main()
{
  vec4  v = vPosition;
  
  // tehdrahedron: give different faces different colours
    if (gl_VertexID % 4 == 0) {
        f_colour = vec4(1,0,0,1);
    } else if (gl_VertexID % 4 == 1) {
        f_colour = vec4(0,1,0,1);
    } else if (gl_VertexID % 4 == 2) {
        f_colour = vec4(0,0,1,1);
    }
    else{
        f_colour = vec4(0,1,1,1);
    }

  gl_Position = Projection * ModelView * v;
}