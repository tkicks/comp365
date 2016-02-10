////////////////////////////////////////////////////////////////////////////////        
// blendRectangles2.cpp
//
// This program builds on blendRectangles1.cpp adding an opaque green rectangle. 
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////// 

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Function to draw a blue rectangle at z=0.
void drawBlueRectangle(void)
{
   glColor4f(0.0, 0.0, 1.0, 0.5); // Translucent: alpha value = 0.5.

   glPushMatrix();
   glTranslatef(0.0, -0.5, 0.0);
   glBegin(GL_POLYGON);
      glVertex3f(-1.0, -0.25, 0.0);
	  glVertex3f(1.0, -0.25, 0.0);
      glVertex3f(1.0, 0.25, 0.0);
	  glVertex3f(-1.0, 0.25, 0.0);
   glEnd();
   glPopMatrix();
}

// Function to draw a green rectangle at z=0.1.
void drawGreenRectangle(void)
{
   glColor4f(0.0, 1.0, 0.0, 1.0); // Opaque: alpha value = 1.0.

   glPushMatrix();
   glTranslatef(0.0, 0.0, 0.1);
   glRotatef(45.0, 0.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
      glVertex3f(-1.0, -0.25, 0.0);
	  glVertex3f(1.0, -0.25, 0.0);
      glVertex3f(1.0, 0.25, 0.0);
	  glVertex3f(-1.0, 0.25, 0.0);
   glEnd();
   glPopMatrix();
}

// Function to draw a red rectangle at z=0.2.
void drawRedRectangle(void)
{
   glColor4f(1.0, 0.0, 0.0, 0.5); // Translucent: alpha value = 0.5.

   glPushMatrix();
   glTranslatef(0.5, 0.0, 0.2);
   glRotatef(90.0, 0.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
      glVertex3f(-1.0, -0.25, 0.0);
	  glVertex3f(1.0, -0.25, 0.0);
      glVertex3f(1.0, 0.25, 0.0);
	  glVertex3f(-1.0, 0.25, 0.0);
   glEnd();
   glPopMatrix();
}

// Initialization routine.
void setup(void)
{  
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   glEnable(GL_BLEND); // Enable blending.
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
}

// Drawing routine.
void drawScene()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   drawRedRectangle(); // Red rectangle closest to viewer, translucent.
   drawGreenRectangle(); // Green rectangle second closest, opaque.
   drawBlueRectangle(); // Blue rectangle farthest, translucent.

   glFlush();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
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
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("blendRectangles2.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMainLoop();
   
   return 0;
}
