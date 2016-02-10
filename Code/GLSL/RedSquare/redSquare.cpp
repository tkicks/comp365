////////////////////////////////////////////////////////////////////
// redSquare.cpp
//
// This program, based on square.cpp, is a simple GLSL program to
// show shader functionality. It adds to square.cpp minimal code
// to activate a vertex shader and a fragment shader. 
// The output is a red square.
//
// Additional GLSL code is indicated with a NEW comment.
//
// This is a MINIMAL program with NO checks at all to see if shaders 
// have been successfully read, compiled and linked.
// 
//
// Sumanta Guha
////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/glut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") // Compiler directive to include the GLEW library.
#endif

using namespace std;

// NEW: Globals:
static int programHandle; // Reference to program object.
static int vertexShaderHandle; // Reference to vertex shader object.
static int fragmentShaderHandle; // Reference to fragment shader object.

// NEW: Function to read a shader file.
char* readShader(char* aShaderFile)
{
   FILE* filePointer = fopen(aShaderFile, "rb");	
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal+1) * sizeof(char)); 
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

// NEW: Function to initialize shaders.
void setShaders(char* vertexShaderFile, char* fragmentShaderFile)  
{
   char* vertexShader = readShader(vertexShaderFile);
   char* fragmentShader = readShader(fragmentShaderFile);

   programHandle = glCreateProgram(); // Creates empty program object and returns reference to it.
   vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER); // Creates empty vertex shader object and returns reference to it.
   fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER); // Creates empty fragment shader object and returns reference to it.

   glShaderSource(vertexShaderHandle, 1, (const char**) &vertexShader, NULL); // Sets source code for vertex shader.
   glShaderSource(fragmentShaderHandle, 1, (const char**) &fragmentShader, NULL); // Sets source code for fragment shader.

   glCompileShader(vertexShaderHandle); // Compiles vertex shader shource code.
   glCompileShader(fragmentShaderHandle); // Compiles fragment shader source code.

   glAttachShader(programHandle, vertexShaderHandle); // Attaches vertex shader to program.
   glAttachShader(programHandle, fragmentShaderHandle); // Attaches fragment shader to program.

   glLinkProgram(programHandle); // Links the program.
   glUseProgram(programHandle); // Installs program into current rendering state.
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(1.0, 1.0, 1.0);

   // Draw square.
   glBegin(GL_POLYGON);
      glVertex3f(20.0, 20.0, 0.0);
      glVertex3f(80.0, 20.0, 0.0);
      glVertex3f(80.0, 80.0, 0.0);
      glVertex3f(20.0, 80.0, 0.0);
   glEnd();

   glFlush();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {	  
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("redSquare.cpp");
   setup();
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   // NEW: Initialize GLEW and invoke the shaders.
   glewInit();
   setShaders("passThrough.vs", "red.fs");

   glutMainLoop();

   return 0;   
}
