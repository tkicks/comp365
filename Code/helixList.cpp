///////////////////////////////////////////////////////////        
// helixList.cpp
//
// This program draws several helixes using a display list.
// 
// Sumanta Guha.
///////////////////////////////////////////////////////////

#include <cstdlib>
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
static unsigned int aHelix; // List index.

// Initialization routine.
void setup(void) 
{
   float t; // Angle parameter.

   aHelix = glGenLists(1); // Return a list index.

   // Begin create a display list.
   glNewList(aHelix, GL_COMPILE);

   // Draw a helix.
   glBegin(GL_LINE_STRIP);
   for(t = -10 * PI; t <= 10 * PI; t += PI/20.0) 
      glVertex3f(20 * cos(t), 20 * sin(t), t);
   glEnd();
   
   glEndList();
   // End create a display list.

   glClearColor(1.0, 1.0, 1.0, 0.0);  
}

// Initialization routine.
void drawScene(void)
{  
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);

   glColor3f(1.0, 0.0, 0.0);
   glPushMatrix();
   glTranslatef(-20.0, 0.0, -70.0);
   glScalef(0.2, 0.5, 1.0);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glColor3f(0.0, 1.0, 0.0);
   glPushMatrix();
   glTranslatef(20.0, 0.0, -60.0);
   glRotatef(30.0, 0.0, 0.0, 1.0);
   glScalef(0.2, 0.3, 1.0);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glColor3f(0.0, 0.0, 1.0);
   glPushMatrix();
   glTranslatef(0.0, 10.0, -50.0);
   glRotatef(-30.0, 0.0, 0.0, 1.0);
   glScalef(0.5, 0.5, 1.0);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glColor3f(1.0, 1.0, 0.0);
   glPushMatrix();
   glTranslatef(0.0, -10.0, -40.0);
   glRotatef(90.0, 0.0, 0.0, 1.0);
   glScalef(0.5, 0.2, 0.5);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glColor3f(1.0, 0.0, 1.0);
   glPushMatrix();
   glTranslatef(-20.0, 20.0, -50.0);
   glRotatef(0.0, 0.0, 0.0, 1.0);
   glScalef(0.5, 0.3, 0.5);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glColor3f(0.0, 1.0, 1.0);
   glPushMatrix();
   glTranslatef(30.0, -30.0, -60.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);
   glScalef(0.5, 0.5, 0.5);
   glCallList(aHelix); // Execute display list.
   glPopMatrix();

   glFlush();
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
   glutCreateWindow("helixList.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}
