////////////////////////////////////////////////////////////////////////////////////
// wavyCylinder2.cpp
//
// This program is a modification of wavyCylinder1.cpp which itself was based on
// cylinder.cpp. The output of the program is exactly the same as that of 
// wavyCylinder1.cpp: it draws a cylinder with a surface whose waviness can be 
// controlled and color can be changed.
// 
// The difference between wavyCylinder1.cpp and the current program is that the former
// program supplied only a parameter to the vertex shader through the uniform 
// variable waveParamShader, which was subsequently used by the shader to compute
// a scale factor by which to multiply the cylinder's cross-section. 
// However, the current program itself computes the scale factor per vertex and sends 
// it to the vertex shader via the attribute variable scaleFactorShader. Subsequently, 
// the shader simply uses this factor to scale the cylinder cross-section.
//
// This program, exactly as wavyCylinder1.cpp, also sends a single parameter, via the 
// uniform variable colorParamShader, to the fragment shader to interpolate the color 
// of the cylinder between red and green.
//
// Interaction:
// Press up/down arrow keys to increase/decrease the number of waves on the cylinder.
// Press left/right arrow keys to change its color.
// Press x, X, y, Y, z, Z to turn the cylinder.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

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

#define PI 3.14159265358979324

using namespace std;

// Globals.
static int p = 60; // Number of grid columns.
static int q = 40; // Number of grid rows
static float *vertices = NULL; // Vertex array of the mapped sample on the cylinder.
static float Xangle = 150.0, Yangle = 60.0, Zangle = 0.0; // Angles to rotate the cylinder.

static int programHandle; // Reference to program object. 
static int vertexShaderHandle; // Reference to vertex shader object.
static int fragmentShaderHandle; // Reference to fragment shader object.

static float waveParamProgram = 2.0; // Program parameter to control waviness of cylinder surface.
static float scaleFactorProgram = 1.0; // Program parameter to scale cylinder vertices 
                                       // (it is echoed by its counterpart variable in the vertex shader). 
static float colorParamProgram = 0.0; // Program interpolation parameter to control color of cylinder surface 
                                      // (it is echoed by its counterpart variable in the fragment shader).

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

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the cylinder.
float f(int i, int j)
{
   return ( cos( (-1 + 2*(float)i/p) * PI ) );
}

float g(int i, int j)
{
   return ( sin( (-1 + 2*(float)i/p) * PI ) );
}

float h(int i, int j)
{
   return ( -1 + 2*(float)j/q );
}

// Routine to fill the vertex array with co-ordinates of the mapped sample points.
void fillVertexArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= q; j++)
      for (i = 0; i <= p; i++)
      {
         vertices[k++] = f(i,j);
         vertices[k++] = g(i,j);
         vertices[k++] = h(i,j);
      }
}

// Initialization routine.
void setup(void)
{
   glEnableClientState(GL_VERTEX_ARRAY);

   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// Drawing routine.
void drawScene(void)
{
   int  i, j;
   vertices = new float[3*(p+1)*(q+1)]; // Dynamic array allocation with new value of p and q. 
   int attributeIndex = 1; // Location of an attribute index.

   glVertexPointer(3, GL_FLOAT, 0, vertices);   
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glColor3f(1.0, 1.0, 1.0);

   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Fill the vertex array.
   fillVertexArray();

   // Associate a user-defined attribute variable with a program attribute index.
   glBindAttribLocation(programHandle, attributeIndex, "scaleFactorShader");

   // Make the approximating triangular mesh.
   for(j = 0; j < q; j++)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i <= p; i++)
      {
         // Scale factor is a sine function along the z-axis.
		 scaleFactorProgram = 1 + 0.1 * sin(waveParamProgram * PI * h(i,j+1));
		 // Per-vertex operation to pass value from a program variable to an attribute variable associated with given index.
		 glVertexAttrib1f(attributeIndex, scaleFactorProgram);	
         glArrayElement( (j+1)*(p+1) + i );
         
		 // Repeat above 3 statements for another vertex.
		 scaleFactorProgram = 1 + 0.1 * sin(waveParamProgram * PI * h(i,j));
		 glVertexAttrib1f(attributeIndex, scaleFactorProgram);
         glArrayElement( j*(p+1) + i );
	  }
      glEnd();
   }

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
   glMatrixMode(GL_MODELVIEW);

}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   int parameterLocation; // Location of a uniform variable.
   
   // Return location of a uniform variable.
   parameterLocation = glGetUniformLocation(programHandle,"colorParamShader"); 
   // Specify that uniform variable at indicated location gets its values from the indicated program variable. 
   glUniform1f(parameterLocation, colorParamProgram); 

   if (key == GLUT_KEY_DOWN) if (waveParamProgram > 2.0) waveParamProgram -= 0.25;
   if (key == GLUT_KEY_UP) waveParamProgram += 0.25;
   if (key == GLUT_KEY_LEFT) colorParamProgram -= 0.05;
   if (key == GLUT_KEY_RIGHT) colorParamProgram += 0.05;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press up/down arrow keys to increase/decrease the number of waves on the cylinder." << endl
        << "Press left/right arrow keys to change the color." << endl
        << "Press x, X, y, Y, z, Z to turn the cylinder." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("wavyCylinder2.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewInit();
   setShaders("waves2.vs", "colorInterpolate.fs");

   glutMainLoop();

   return 0;
}

