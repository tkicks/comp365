//////////////////////////////////////////////////////////////////////////////////////////
// bumpMappingPerVertexLighting.cpp
//
// This program, based on bumpMapping.cpp, shows bump mapping by perturbing the normals 
// of a plane triangular grid to make it appear corrugated. However, now the shaders
// implement per-vertex lighting following Phong's lighting model, and then Gouraud 
// shading to interpolate colors through each triangle. This replicates exactly OpenGL's 
// native lighting calculation.
//
// Interaction:
// Press space to toggle between bump mapping on and off.
//
// Note: The programs bumpMappingPerVertexLighting.cpp and bumpMappingPerPixelLighting.cpp
//       are exactly same. Only the shaders are different.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////////////////////

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

using namespace std;

// Globals.
static int isBumpMapped = 0; // Is bump mapping on?

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

// Initialization routine.
void setup(void)
{    
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING); 

   // Automatically resize all normals to unit length.
   glEnable(GL_NORMALIZE); 

   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 0.0 };
   float lightPos[] = {0.0, 1.0, 0.0, 0.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

	// Material property vectors.
   float matAmbAndDif[] = {0.0, 0.5, 0.5, 1.0};
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
}

// Drawing routine.
void drawScene(void)
{
   int i = 0;
  
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 5.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Draw a plane triangular grid. 
   for(float v = 100.0; v > -100.0; v -= 1.0)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(float u = -100.0; u < 100.0; u += 1.0)
	  {
         if (isBumpMapped) glNormal3f(2.0*cos(2.0*u), 1.0, 0.0); // Bump-mapped normals.
		 else glNormal3f(0.0, 1.0, 0.0); // Original vertical normals.
		 glVertex3f(u, 0.0, v - 1.0);
	     glVertex3f(u, 0.0, v);
		 i++;
	  }
      glEnd();
	  i++;
   }

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
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
      case ' ':
         if (isBumpMapped) isBumpMapped = 0;
		 else isBumpMapped = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between bump mapping on and off." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("bumpMappingPerVertexLighting.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewInit();
   setShaders("perVertexLightingSimple.vs", "passThrough.fs");

   glutMainLoop();

   return 0; 
}
