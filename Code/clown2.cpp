////////////////////////////////////////////////////////////////////////////      
// clown2.cpp
// 
// This program is the second step in drawing a clown's head. See Chapter 4.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////// 

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
static float angle = 0.0; // Rotation.angle of hat.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

  // Place scene in frustum.
   glTranslatef(0.0, 0.0, -9.0);

   // Head.
   glColor3f(0.0, 0.0, 1.0);
   glutWireSphere(2.0, 20, 20);

   // Transformations of the hat and brim.
   glRotatef(angle, 0.0, 1.0, 0.0);
   glRotatef(30.0, 0.0, 0.0, 1.0);
   glRotatef(-90.0, 1.0, 0.0, 0.0);
   glTranslatef(0.0, 0.0, 2.0);

   // Hat.
   glColor3f(0.0, 1.0, 0.0);
   glutWireCone(2.0, 5.0, 20, 20);

   // Brim.
   glutWireTorus(0.2, 2.2, 10, 25);

   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      angle += 5.0;
	  if (angle > 360.0) angle -= 360.0;
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
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
         if (isAnimate) isAnimate = 0;
		 else isAnimate = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) animationPeriod += 5;
   if( key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow ("clown2.cpp"); 
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop(); 

   return 0;  
}

