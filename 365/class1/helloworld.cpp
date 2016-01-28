/* 
 * helloclass.cpp 
 *
 * Draws a simple triangle and allows user to interactively draw small 
 * rectangles in the direction of values on the numeric keypad.
 * Other keyboard functions can be seen in keyboard function below.
 */

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

   /* set color - this happens to be blue */
   glColor3f (0.0, 0.0, 1.0);

   /* draw the polygon */
   glBegin (GL_POLYGON);
      glVertex3f (0.5, 0.75, 0.0);
      glVertex3f (0.25, 0.25, 0.0);
      glVertex3f (0.75, 0.25, 0.0);
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

void drawpoly (float x, float y)
/* draw a polygon */
/* Here's how this stuff is weird: This is called from keyboard.
 * When the user presses 'd', this polygon is drawn.  This will work
 * at any time because the keyboard function is called repeatedly.
 * HOWEVER, drawpoly () is done ONLY when 'd' is pressed.  Things will
 * work as you expect UNTIL YOU COVER THE WINDOW.  At this point, it
 * has to redraw, and it won't remember that you hit 'd' and the
 * little polygon will go away.
 */
{
   glColor3f (0.5, 0.0, 0.5);
   glBegin (GL_POLYGON);
      glVertex3f (x, y, 0.0);
      glVertex3f (x, y+0.02, 0.0);
      glVertex3f (x+0.02, y+0.02, 0.0);
      glVertex3f (x+0.02, y, 0.0);
   glEnd();

   /* display the new polygon NOW */
   glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
   /* x and y above is from cursor position on window */

   switch (key) {
      case 'e' : glutPostRedisplay();
                 break;
      case 'd' : drawpoly (0.2, 0.2);
                 cpx = 0.2;
                 cpy = 0.2;
                 break;
      case 'c' : glClear (GL_COLOR_BUFFER_BIT);
                 glFlush ();
                 break;
      case '4' : cpx -= 0.05;
                 drawpoly (cpx, cpy);
                 break;
      case '8' : cpy += 0.05;
                 drawpoly (cpx, cpy);
                 break;
      case '6' : cpx += 0.05;
                 drawpoly (cpx, cpy);
                 break;
      case '2' : cpy -= 0.05;
                 drawpoly (cpx, cpy);
                 break;
                 
      /* 27 = <esc> */
      case 27  : 
      case 'q' : exit (1);    // either <esc> or 'q' will quit program
   }
}


int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   /* set up the window size, in pixels */
   glutInitWindowSize (750, 250); 

   /* put the window at 100, 100 on user's screen */
   glutInitWindowPosition (100, 100);

   /* make a title for window */
   glutCreateWindow ("Welcome to Computer Graphics");

   /* set up some GL stuff */
   init ();

   /* register callback functions */

   /* check the keyboard continuously by calling keyboard function */
   glutKeyboardFunc(keyboard);

   /* call display function */
   glutDisplayFunc(display); 

   cout << "OpenGL version: " << (char *) glGetString (GL_VERSION) << endl;
   cout << "GLSL version  : " << (char *) glGetString (GL_SHADING_LANGUAGE_VERSION) << endl;

   /* repeat these things indefinitely */
   glutMainLoop();

   /* we're done! */
   return 0; 
}