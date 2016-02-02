#include <iostream>
#include <GL/glut.h>

using namespace std;

/* globals for current position (CP) */
float cpx = 0.2;
float cpy = 0.2;;

void display ()
{
   /* this function is always called so this stuff will always be drawn */

   /* set up the window */
   glClear (GL_COLOR_BUFFER_BIT);

   /* set color of house to white */
   glColor3f (1.0, 1.0, 1.0);

   /* draw the roof */
   glBegin (GL_POLYGON);
      glVertex3f (0.15, 0.8, 0.0);
      glVertex3f (0.25, 0.9, 0.0);
      glVertex3f (0.35, 0.8, 0.0);
   glEnd();

   /* set color of separator to black */
   glColor3f(0.0, 0.0, 0.0);

   /* set width of separator to 2 */
   glLineWidth(2);

   /* draw separator between roof and body of house */
   glBegin(GL_LINES);
      glVertex3f(0.15, 0.8, 0.0);
      glVertex3f(0.35, 0.8, 0.0);
   glEnd();

   /* set color of house to white */
   glColor3f (1.0, 1.0, 1.0);

   /* draw the body of house */
   glRectf(0.15, 0.8, 0.35, 0.5);

   /* set color of separator to black */
   glColor3f(0.0, 0.0, 0.0);

   /* draw separator between body of house and garage */
   glBegin(GL_LINES);
      glVertex3f(0.35, 0.8, 0.0);
      glVertex3f(0.35, 0.35, 0.0);
   glEnd();

   /* set color of house to white */
   glColor3f (1.0, 1.0, 1.0);

   /* draw the garage */
   glRectf(0.35, 0.7, 0.6, 0.5);

   /* set color of separator to black */
   glColor3f(0.0, 0.0, 0.0);

   /* draw separator between garage and roof */
   glBegin(GL_LINES);
      glVertex3f(0.35, 0.7, 0.0);
      glVertex3f(0.6, 0.7, 0.0);
   glEnd();

   /* set color of house to white */
   glColor3f (1.0, 1.0, 1.0);

   /* draw the roof over garage */
   glBegin(GL_POLYGON);
      glVertex3f(0.35, 0.7, 0.0);
      glVertex3f(0.475, 0.8, 0.0);
      glVertex3f(0.6, 0.7, 0.0);
   glEnd();

   /* flush the buffer (i.e., draw the thing!) */
   glFlush ();
}

void init ()
{
   /* i.e., black */
   glClearColor (0.0, 0.0, 0.0, 0.0);

   /* type of projection */
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   /* set up viewport 0 to 1 (x and y) and orthographic projection -1 to 1 */
   glOrtho (0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

   // set up window size
   glutInitWindowSize(1000, 800);

   // put window near top left of screen
   glutInitWindowPosition(100, 100);

   // make title for window
   glutCreateWindow("My house");

   // set up GL
   init();

   // display
   glutDisplayFunc(display);

   // repeat to keep showing
   glutMainLoop();

   return 0;
}