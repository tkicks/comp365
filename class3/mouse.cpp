// mouse.cpp
// Demo showing use of the mouse, version 1.
//

using namespace std;

#include <stdio.h>
#include <GL/glut.h>
#include <iostream>

int windowHeight = 400;
int windowWidth = 400;

void display ()
{
   int i;

   glClear (GL_COLOR_BUFFER_BIT);

   glFlush ();
}

void myMouse (int button, int state, int x, int y)
/* Function to draw points and lines using the mouse 
 * Left button - draws point 
 * Right button - draws line from previous right button point
 * Center (both) button - stops line drawing 
 *
 * Note that mouse y position is 0 at TOP of window
 * Note also that if the window gets covered, OpenGL won't remember 
 * all the lines/points that were drawn previously 
 */
{
   static int lastx = -1;
   static int lasty;

   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      glColor3f (0.0, 0.4, 1.0);
      glBegin(GL_POINTS);
         glVertex2i(x, windowHeight-y);         // -y b/c openGL flips system coordinate system
         glVertex2i(x-1, windowHeight-y);       // -y b/c openGL flips system coordinate system
         glVertex2i(x+1, windowHeight-y);       // -y b/c openGL flips system coordinate system
         glVertex2i(x, windowHeight-y+1);       // -y b/c openGL flips system coordinate system
         glVertex2i(x, windowHeight-y-1);       // -y b/c openGL flips system coordinate system
      glEnd();
   } 
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
      if (lastx < 0) {
         lastx = x;
         lasty = windowHeight - y;
      }
      else {
         glColor3f (1.0, 0.0, 0.0);
         glBegin(GL_LINE_STRIP);
            glVertex2i (lastx, lasty);
            glVertex2i (x, windowHeight - y);
         glEnd();
         lastx = x;
         lasty = windowHeight - y;
      }
   }
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
      lastx = -1;
   }

   cout << lastx << ", " << lasty << endl;
         
   glFlush();
}

void init ()
{
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   glClearColor (0.8, 0.8, 0.8, 0.0);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   
   gluOrtho2D (0.0, windowWidth, 0.0, windowHeight);
}

int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutInitWindowSize (400, 400); 

   glutInitWindowPosition (100, 100);

   glutCreateWindow ("mouse");

   init ();

   glutDisplayFunc(display); 

   // mouse function registration
   glutMouseFunc (myMouse);

   glutMainLoop();

   return 0; 
}
