/* moveTeapot.cpp
 *
 * Starting code for in-class exercise to animate the teapot.
 *
 */

#include <iostream>
#include <GL/glut.h>

using namespace std;

const float ROT_INCR = 5.0;     /* rotation increment */
long  NUM_TICKS = 25000;  /* idle time between animation frames */
            /* This value is system dependent */

static float Rot = 0.0;    /* rotation angle */
static float zVal = 0;        /* translation in z */
static int   z_incr;       /* z increment */

float z = 5.0;  // distance of eye (center of projection) from frustum

static void Idle( void )

/* This is used to slow down your animation, if necessary.  I used it as
 * a debugging tool - see the commented out parts.  I also used it to 
 * change the z value used in the animation.
 */

{
   clock_t endWait;  /* end of wait interval */

   Rot += ROT_INCR;  /* update the rotation angle */

   if (zVal < 1)        /* update the z translation value */
      z_incr = 1;
   else if (zVal > 15)
      z_incr = -1;
   zVal += z_incr;

   // cin.get();              /* take out comments for debugging - slows down */
                              /* the animation; must press return to continue */

   /* a better way to wait: */
   endWait = clock () + NUM_TICKS;
   /* please wait...*/
   while (clock () < endWait);

   glutPostRedisplay();       /* call Display function again */
}

void display ()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();
   gluLookAt (0.0, 0.0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glScalef (1.0, 1.0, 1.0);

   /* draw a red cube in center of volume */
   glColor3f (1.0, 0.0, 0.0);
   glutWireCube (0.8);

   /* draw the teapot */
   glPushMatrix();
      glRotatef(Rot, 0, 1, 0);
      glColor3f (0.2, 0.2, 1.0);
      glTranslatef (2.0, 0.0, 0.0);
      glutWireTeapot (0.6);
   glPopMatrix();

   glutSwapBuffers();
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
   glFrustum (-1, 1, -1, 1, 1.5, 20.0);

   /* reset matrices to user's coordinate system */
   glMatrixMode (GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case '8': z = z + 1;
                glutPostRedisplay ();
                break;
      case '2': z = z - 1;
                glutPostRedisplay ();
                break;
      case 'q': exit (1);
   }
}

int main (int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Spinning Teapot");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard);
   glutIdleFunc(Idle);
   glutMainLoop();
   return 0; 
}
