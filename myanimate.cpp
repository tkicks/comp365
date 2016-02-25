/* myanimate.cpp
 * 
 * This program shows how to do simple animation.  This is accomplished by
 * loading up a frame buffer while the previous one is being drawn.  The
 * function call glutSwapBuffers() then switches the buffers, drawing the
 * new one, and the process repeats.
 *
 * The program also shows how to work with the CTM, either by continually 
 * changing it or by continually using the initial one.  See comments below
 * for more information.
 */

#include <time.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>

using namespace std;

const float ROT_INCR = 5.0;     /* rotation increment */
long  NUM_TICKS = 15000;  /* idle time between animation frames */
				/* This value is system dependent */

static float Rot = 0.0;   	/* rotation angle */
static float z = 0;       	/* translation in z */
static float r = 0;       	/* starting color values */
static float g = 0;
static float b = 1;
static int   z_incr;      	/* z increment */

static void Idle( void )

/* This is used to slow down your animation, if necessary.  I used it as
 * a debugging tool - see the commented out parts.  I also used it to 
 * change the z value used in the animation.
 */

{
   clock_t endWait;  /* end of wait interval */

   Rot += ROT_INCR;  /* update the rotation angle */

   if (z < 1)        /* update the z translation value */
      z_incr = 1;
   else if (z > 15)
      z_incr = -1;
   z += z_incr;

   // cin.get();              /* take out comments for debugging - slows down */
                              /* the animation; must press return to continue */

   /* a better way to wait: */
   endWait = clock () + NUM_TICKS;
   /* please wait...*/
   while (clock () < endWait);

   glutPostRedisplay();       /* call Display function again */
}


static void Display( void )
{
   /* Push/Pop are needed because we keep incrementing the rotation.
    * Another way to do this would be to NOT push/pop, but then NOT keep
    * updating the rotation, but rather just keep rotating the CTM by some
    * fixed number of degrees.
    */

   glClear( GL_COLOR_BUFFER_BIT );  	/* clears the frame buffer! */

   glPushMatrix();              	/* this stores the CTM */
   glRotatef(Rot, 0, 0, 1); 
   glTranslatef (0.0, 0.0, z);  	/* translating in z */
                              
   glColor3f (r, g, b);
   g+=0.01;
   if (g > 1)
      r+= 0.01;
   if (r > 1)
      b-= 0.01;

   glBegin(GL_POLYGON);
      glTexCoord2f(0, 1);  glVertex2f(-1, -1);
      glTexCoord2f(1, 1);  glVertex2f( 1, -1);
      glTexCoord2f(1, 0);  glVertex2f( 1,  1);
      glTexCoord2f(0, 0);  glVertex2f(-1,  1);
   glEnd();

   glPopMatrix();        		/* brings back the previous CTM */

   /* this displays the new changes */
   glutSwapBuffers();
}

static void Display2( void )
{
   /* This does the same as Display(), but without using Push/Pop.
    * Substitute Display2 for Display in glutDisplayFunc() in main() to
    * try this out.  (Note that this function only does the rotation and color)
    */

   float RotAngle;

   RotAngle = ROT_INCR;
   glClear( GL_COLOR_BUFFER_BIT );

   glRotatef(RotAngle, 0, 0, 1);

   glColor3f (r, g, b);
   g+=0.01;
   if (g > 1)
      r+= 0.01;
   if (r > 1)
      b-= 0.01;

   glBegin(GL_POLYGON);
      glTexCoord2f(0, 1);  glVertex2f(-1, -1);
      glTexCoord2f(1, 1);  glVertex2f( 1, -1);
      glTexCoord2f(1, 0);  glVertex2f( 1,  1);
      glTexCoord2f(0, 0);  glVertex2f(-1,  1);
   glEnd();

   glutSwapBuffers();
}



static void Reshape( int width, int height )
{
   glViewport( 0, 0, width, height );
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();                            /* sets the initial CTM */
   glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 25.0 );/* what the heck is "Frustum"?*/
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glTranslatef( 0.0, 0.0, -25.0 );  	/* move object back into frustum */
				     	/* smaller number moves it INTO screen */
}


static void Key( unsigned char key, int x, int y )
{
   switch (key) {
      case 'f': if (NUM_TICKS > 100)
                  NUM_TICKS -= 1000;
               break;
      case 's': if (NUM_TICKS < 55000)
                  NUM_TICKS += 1000;
               break;
      case 'q':
      case 27 :
         exit(0);
         break;
   }
   glutPostRedisplay();  /* call Display function again */
}



int main( int argc, char *argv[] )
{
   glutInit( &argc, argv );
   glutInitWindowPosition( 0, 0 );
   glutInitWindowSize( 400, 400 );

   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );

   glutCreateWindow(argv[1]);   /* makes window name the name of executable */
				/* uh, what? */

   glutReshapeFunc( Reshape );
   glutKeyboardFunc( Key );
   glutDisplayFunc( Display );  /* can also call Display2 */
   glutIdleFunc( Idle );

   glutMainLoop();
   return 0;
}
