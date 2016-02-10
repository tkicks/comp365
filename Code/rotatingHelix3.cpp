//////////////////////////////////////////////////////////////////////         
// rotatingHelix3.cpp
//
// This program, based on helix.cpp, animates a helix by rotating
// it around its axis using a timer function called by glutTimerFunc().
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
//
//Sumanta Guha.
////////////////////////////////////////////////////////////////////// 

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
static int isAnimate = 0; // Animated?
static int animationPeriod = 50; // Time interval between frames.
static float angle = 0.0; // Angle of rotation.

// Drawing routine.
void drawScene(void)
{  
   float R = 20.0; // Radius of helix.

   float t; // Angle parameter along helix.

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   glPushMatrix();

   // The Trick: to align the axis of the helix along the y-axis prior to rotation
   // and then return it to its original location.
   glTranslatef(0.0, 0.0, -60.0);
   glRotatef(angle, 0.0, 1.0, 0.0);
   glTranslatef(0.0, 0.0, 60.0);

   glBegin(GL_LINE_STRIP);
   for(t = -10 * PI; t <= 10 * PI; t += PI/20.0) 
      glVertex3f(R * cos(t), t, R * sin(t) - 60.0);
   glEnd();
   
   glPopMatrix();
   glutSwapBuffers();
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
   glLoadIdentity();
}

// Routine to increase the rotation angle.
void increaseAngle(void)
{
   angle += 5.0;
   if (angle > 360.0) angle -= 360.0;
   glutPostRedisplay();
}

// Timer function for glutTimerFunc().
// The parameter someValue(=1) is passed to animate by glutTimerFunc(), 
// but is not used.
void animate(int someValue)
{
   if (isAnimate) increaseAngle();

   // Note that glutTimerFunc() calls animate() *once* after the 
   // specified msecs. Therefore, to make repeated calls, animate() 
   // must call itself again with glutTimerFunc() as below.
   glutTimerFunc(animationPeriod, animate, 1);
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
         if(isAnimate) isAnimate = 0;
		 else isAnimate = 1;
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) animationPeriod +=5;
   if( key == GLUT_KEY_UP) 
      if (animationPeriod > 5) animationPeriod -=5;
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
   glutCreateWindow("rotatingHelix3.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   // Register the timer function animate().
   // The timer function is called after 5 msecs. with 
   // the parameter value 1 passed to it.
   glutTimerFunc(5, animate, 1);

   glutMainLoop(); 

   return 0;  
}
