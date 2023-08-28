// Assignment 1 example based on Example 6
// NOTE: This is not a complete model solution!

#include "common.h"
#include <chrono>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char *WINDOW_TITLE = "Assignment 1 Example";
const double FRAME_RATE_MS = 1000.0/60.0;

typedef glm::vec4  color4;
typedef glm::vec4  point4;

// Tetrahedron
point4 vertices[4] = {
   point4(  0.0,  0.0,  0.0, 1.0 ),
   point4( -0.5, -1.0, -0.5, 1.0 ),
   point4(  0.5, -1.0, -0.5, 1.0 ),
   point4(  0.0, -1.0,  0.5, 1.0 )
};

GLuint indices[] = {
   // each group of 3 is a triangle face
   0, 1, 2,
   0, 1, 3,
   0, 3, 2,
   1, 2, 3
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { XAxis = 0, YAxis = 1, ZAxis = 2, NumAxes = 3 };
bool X_axis = false, Y_axis = true, Z_axis = false;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint  ModelView, Projection, Color, T;

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
   // Create a vertex array object
   GLuint vao = 0;
   glGenVertexArrays( 1, &vao );
   glBindVertexArray( vao );

   GLuint buffer;

   // Create and initialize a buffer object
   glGenBuffers( 1, &buffer );
   glBindBuffer( GL_ARRAY_BUFFER, buffer );
   glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

   // Another for the index buffer
   glGenBuffers( 1, &buffer );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

   // Load shaders and use the resulting shader program
   GLuint program = InitShader( "vshaderA1.glsl", "fshaderA1.glsl" );
   glUseProgram( program );

   // set up vertex arrays
   GLuint vPosition = glGetAttribLocation( program, "vPosition" );
   glEnableVertexAttribArray( vPosition );
   glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

   ModelView = glGetUniformLocation( program, "ModelView" );
   Projection = glGetUniformLocation( program, "Projection" );
   Color = glGetUniformLocation( program, "Color" );
   T = glGetUniformLocation( program, "T" );

   glEnable( GL_DEPTH_TEST );
   glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void drawTetrahedron() {
  // This is the *slowest* way I could think of drawing these shapes.
  // I'm doing it this way to see how many can be drawn...
  glm::vec4 colour(1,0,0,1);
  glUniform4fv( Color, 1, glm::value_ptr(colour) );
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));
  colour = glm::vec4(0,1,0,1);
  glUniform4fv( Color, 1, glm::value_ptr(colour) );
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));
  colour = glm::vec4(0,0,1,1);
  glUniform4fv( Color, 1, glm::value_ptr(colour) );
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));
  colour = glm::vec4(1,0,1,1);
  glUniform4fv( Color, 1, glm::value_ptr(colour) );
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(9 * sizeof(GLuint)));
}

glm::mat4 scale_trans, view, model_view;
int max_depth = 0;
void drawSierpinskiTetrahedron(int curr_depth, int max_depth, glm::mat4 model) {
  if (curr_depth < max_depth) {
    drawSierpinskiTetrahedron(curr_depth+1, max_depth, model * glm::scale(glm::mat4(), glm::vec3(0.5, 0.5, 0.5)));
    drawSierpinskiTetrahedron(curr_depth+1, max_depth, model * glm::scale(glm::mat4(), glm::vec3(0.5, 0.5, 0.5)) * glm::translate(glm::mat4(), glm::vec3(-0.5, -1, -0.5)));
    drawSierpinskiTetrahedron(curr_depth+1, max_depth, model * glm::scale(glm::mat4(), glm::vec3(0.5, 0.5, 0.5)) * glm::translate(glm::mat4(), glm::vec3(0, -1, 0.5)));
    drawSierpinskiTetrahedron(curr_depth+1, max_depth, model * glm::scale(glm::mat4(), glm::vec3(0.5, 0.5, 0.5)) * glm::translate(glm::mat4(), glm::vec3(0.5, -1, -0.5)));
  } else {
    model_view = view * scale_trans * model;
    glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );
    drawTetrahedron();
  }
}

long long start_time = -1;
void
display( void )
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   //  Generate the model-view matrix

   const glm::vec3 viewer_pos( 0.0, 0.0, 2.0 );
   glm::mat4 trans, rot;
   //glm::mat4 model, view, model_view;
   trans = glm::translate(trans, -viewer_pos);
   rot = glm::rotate(rot, glm::radians(Theta[XAxis]), glm::vec3(1,0,0));
   rot = glm::rotate(rot, glm::radians(Theta[YAxis]), glm::vec3(0,1,0));
   rot = glm::rotate(rot, glm::radians(Theta[ZAxis]), glm::vec3(0,0,1));
   view = trans * rot;
   
   //  Set up the timer
   long long ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
   float t = 0.0;
   if (start_time > 0) {
     long long diff = ms - start_time;
     // period for t = 2 sec
     if (diff <= 2000) {
       t = diff / 2000.0;
     } else if (diff <= 4000) {
       t = (4000-diff) / 2000.0;
     } else {
       start_time = ms;
     }
   } else {
     start_time = ms;
   }
   std::cout << t << std::endl;
   glUniform1f( T, t );

   drawSierpinskiTetrahedron(0, max_depth, glm::translate(glm::mat4(), glm::vec3(0, 0.5, 0)));

   // model = glm::scale(model, glm::vec3(0.5,0.5,0.5));
   // model_view = view * glm::translate(glm::mat4(), glm::vec3(0,0.25,0)) * model;
   // glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );
   // drawTetrahedron();
   //
   // model_view = view * glm::translate(glm::mat4(), glm::vec3(-0.25,-0.25,-0.25)) * model;
   // glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );
   // drawTetrahedron();
   //
   // model_view = view * glm::translate(glm::mat4(), glm::vec3(0,-0.25,0.25)) * model;
   // glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );
   // drawTetrahedron();
   //
   // model_view = view * glm::translate(glm::mat4(), glm::vec3(0.25,-0.25,-0.25)) * model;
   // glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );
   // drawTetrahedron();

   glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
       case 033: // Escape Key
       case 'q': case 'Q':
          exit( EXIT_SUCCESS );
          break;
    }
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
       switch( button ) {
          case GLUT_LEFT_BUTTON:    X_axis = !X_axis;  break;
          case GLUT_MIDDLE_BUTTON:  Y_axis = !Y_axis;  break;
          case GLUT_RIGHT_BUTTON:   Z_axis = !Z_axis;  break;
       }
    }
}

//----------------------------------------------------------------------------

int counter = 0;
void
update( void )
{
  if (X_axis) {
    Theta[XAxis] += 0.5;

    if ( Theta[XAxis] > 360.0 ) {
       Theta[XAxis] -= 360.0;
    }
  }
  if (Y_axis) {
    Theta[YAxis] += 0.5;

    if ( Theta[YAxis] > 360.0 ) {
       Theta[YAxis] -= 360.0;
    }
  }
  if (Z_axis) {
    Theta[ZAxis] += 0.5;

    if ( Theta[ZAxis] > 360.0 ) {
       Theta[ZAxis] -= 360.0;
    }
  }
  
  counter++;
  if (90 == counter) {
    counter = 0;
    max_depth++;
    if (max_depth == 7) {
      max_depth = 0;
    }
  }
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
   glViewport( 0, 0, width, height );

   GLfloat aspect = GLfloat(width)/height;
   glm::mat4  projection = glm::perspective( glm::radians(45.0f), aspect, 0.5f, 3.0f );

   glUniformMatrix4fv( Projection, 1, GL_FALSE, glm::value_ptr(projection) );
}
