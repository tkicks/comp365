/////////////////////////////////////////////////////////         
// viewports.cpp
//
// This program shows two viewports in one OpenGL window.
//
// Sumanta Guha.
///////////////////////////////////////////////////////// 

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
float topLeft1, topLeft2;     // window coords each viewport

// Drawing routine.
void drawScene(void)
{  
   glClear(GL_COLOR_BUFFER_BIT);

   // Define first viewport.
   glViewport(0, 0, width/2.0,  height); 
   topLeft1 = 0;
   // End contents of first viewport.

   // Define second viewport.
   glViewport(width/2.0, 0, width/2.0, height);
   topLeft2 = width/2.0;

   // A vertical black line at the left to separate the viewports.
   glColor3f(0.0, 0.0, 0.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
      glVertex3f(0.1, 0.0, 0.0);
	  glVertex3f(0.1, 100.0, 0.0);
   glEnd();
   glLineWidth(1.0);
   // End contents of second viewport.

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
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Pass the size of the OpenGL window to globals.
   width = w;
   height = h;
}

// Keyboard input processing routine.
void mouse (int button, int state, int x, int y)
{
   if (x < topLeft2)
      cout << "left side\n";
   else
      cout << "right side\n"

}

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("viewports.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutMouseFunc(mouse);
   glutMainLoop(); 

   return 0;  
}

