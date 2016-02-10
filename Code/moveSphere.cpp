///////////////////////////////////////////////////////////////
// moveSphere.cpp
//
// This program allows the user to move a sphere to demonstrate
// distortion at the edges of the viewing frustum.
//
// Interaction:
// Press the arrow keys to move the sphere.
// Press the space bar to rotate the sphere..
// Press r to reset.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals.
static float Xvalue = 0.0, Yvalue = 0.0; // Co-ordinates of the sphere.
static float Angle = 0.0; // Angle to rotate the sphere.

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();

   // Set the position of the sphere.
   glTranslatef(Xvalue, Yvalue, -5.0);
   glRotatef(Angle, 1.0, 1.0, 1.0);

   glColor3f(0.0, 0.0, 0.0);
   glutWireSphere(0.5, 16, 10);

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
   glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 'r':
         Xvalue = Yvalue = Angle = 0.0;
         glutPostRedisplay();
         break;
      case ' ':
         Angle += 10.0;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
	  default:
	     break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) Yvalue += 0.1;
   if(key == GLUT_KEY_DOWN) Yvalue -= 0.1;
   if(key == GLUT_KEY_LEFT) Xvalue -= 0.1;
   if(key == GLUT_KEY_RIGHT) Xvalue += 0.1;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the arrow keys to move the sphere." << endl
        << "Press the space bar to rotate the sphere." << endl
        << "Press r to reset." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("moveSphere.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   // Register the callback function for non-ASCII key entry.
   glutSpecialFunc(specialKeyInput);

   glutMainLoop(); 

   return 0;  
}
