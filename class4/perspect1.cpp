/* perspect1.cpp
 *
 * Updated 2/10/2016
 *
 * Shows how to use frustum 
 * (taken from page 98 in OpenGL (1.1) text or page 100 in OpenGL (1.2) text) 
 * User can change the value of the eye position by pressing 8 (moving
 * farther away) and 2 (moving closer) on keyboard.
 *
 * Experiment with this code by creating different polygons and changing
 * the center of projection position until you are sure you understand how
 * the frustum works.
 */

#include <iostream>
#include <GL/glut.h>

using namespace std;

float z = 5.0;  // distance of eye (center of projection) from frustum

void display ()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity ();

   gluLookAt (0.0, 0.0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   /*         -----------  -------------  -------------
    *         |            |              +-> view up vector, here signifying
    *         |            |                  that +y is up (default)
    *         |            +-> view reference point - "look-at point",
    *         |                usually near the center of the scene
    *         +-> origin of viewing reference frame - the "camera" position,
    *             pointing towards the look-at point
    */

   /* scale objects here (if needed) to make sure they fit into frustum */
   glScalef (2.0, 2.0, 1.0);

   glColor3f (1.0, 0.0, 0.0);
   /* draw a polygon */
   glBegin (GL_POLYGON);
      glVertex3f (-0.5, 0.0, 1.0);
      glVertex3f (0.0, 0.5, 1.0);
      glVertex3f (0.5, 0.0, 1.0);
   glEnd();

   glColor3f (0.0, 0.0, 1.0);
   /* draw a unit cube (centered at 0,0,0) */
   glScalef (0.5, 0.5, 1.0);
   glutWireCube (1.0);
   //glutWireTeapot (0.7);
   //glutWireTorus (0.2, 1.0, 10, 50);


   glColor3f(0.0, 0.5, 0.0);
   glBegin(GL_POLYGON);
    glVertex3f(-0.3, -0.3, 0.4);
    glVertex3f(0.3, 0.3, 0.4);
    glVertex3f(-0.3, -0.2, 0.4);
   glEnd();

   glScalef(2.0, 2.0, 1.0);
   glColor3f(0.0, 0.5, 0.0);
   glBegin(GL_POLYGON);
    glVertex3f(-0.3, -0.3, 0.4);
    glVertex3f(1.9, 0.3, 0.4);
    glVertex3f(-0.3, -0.2, 0.4);
   glEnd();

   glFlush ();
}

void init ()
{
   glClearColor (0.8, 0.8, 0.8, 0.0);
   glShadeModel (GL_FLAT);
   glEnable (GL_DEPTH_TEST);
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   /* set up matrices for projection coordinate system */
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   /* Frustum normally defined as -1 to 1 in x, y and a larger spread in z */
   glFrustum (-1, 1, -1, 1, 1.5, 20.0);
   // glOrtho (-1, 1, -1, 1, 1.5, 20.0);

/***************************************************

Remember this is a right-handed system, so z is like this:

+z <-------------------------------------------------------> -z  screen is here

With the call to gluLookAt(), the "eye" (center of projection) is set at 5 in z:

           O
+z <-------------------------------------------------------> -z  screen is here
          +5

Frustum is set up so that the near plane is A DISTANCE of 1.5 from the eye
position and the far plane is A DISTANCE of 20.0 from the eye:

                                     | far plane
               | near plane          | 
           O   |                     |
+z <-------------------------------------------------------> -z  screen is here
          +5  3.5                   -15
           ^---^
            1.5
           ^-------------------------^
                     20.0


This means that the objects must be between 3.5 and -15 in Z (see triangle
polygon above, for example). 

***************************************************/

   /* reset matrices to user's coordinate system */
   glMatrixMode (GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case '8': z = z + 1;
		cout << "z = " << z << endl;
                glutPostRedisplay ();
                break;
      case '2': z = z - 1;
		cout << "z = " << z << endl;
                glutPostRedisplay ();
                break;
      case 'q': exit (1);
   }
}

int main (int argc, char** argv) {
   glutInit(&argc, argv);
   // glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Frustum Fun");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0; 
}
