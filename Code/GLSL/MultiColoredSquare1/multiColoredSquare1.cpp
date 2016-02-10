////////////////////////////////////////////////////////////////
// multiColoredSquare1.cpp
//
// This program is the same as redSquare.cpp, except that the
// vertices are now differently colored and the  vertex shader 
// uses the program-defined gl_Color.
//
// Sumanta Guha
////////////////////////////////////////////////////////////////

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

// Globals.
static int programHandle; // Reference to program object.
static int vertexShaderHandle; // Reference to vertex shader object.
static int fragmentShaderHandle; // Reference to fragment shader object.

// Function to read a shader file.
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

// Function to initialize shaders.
void setShaders(char* vertexShaderFile, char* fragmentShaderFile)  
{
   char* vertexShader = readShader(vertexShaderFile);
   char* fragmentShader = readShader(fragmentShaderFile);

   programHandle = glCreateProgram();
   vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
   fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

   glShaderSource(vertexShaderHandle, 1, (const char**) &vertexShader, NULL);
   glShaderSource(fragmentShaderHandle, 1, (const char**) &fragmentShader, NULL);

   glCompileShader(vertexShaderHandle);
   glCompileShader(fragmentShaderHandle);

   glAttachShader(programHandle, vertexShaderHandle);
   glAttachShader(programHandle, fragmentShaderHandle);

   glLinkProgram(programHandle);
   glUseProgram(programHandle);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(1.0, 1.0, 1.0);

   // Draw square.
   glBegin(GL_POLYGON);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(20.0, 20.0, 0.0);
      glColor3f(0.0, 1.0, 0.0);
      glVertex3f(80.0, 20.0, 0.0);
      glColor3f(0.0, 0.0, 1.0);
      glVertex3f(80.0, 80.0, 0.0);
      glColor3f(1.0, 1.0, 0.0);
      glVertex3f(20.0, 80.0, 0.0);
   glEnd();

   glFlush();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_VERTEX_PROGRAM_TWO_SIDE);
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
   glutCreateWindow("multiColoredSquare1.cpp");
   setup();
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewInit();
   setShaders("simpleColorizer.vs", "passThrough.fs");

   glutMainLoop();

   return 0;   
}
