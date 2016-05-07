///////////////////////////////////////////////////////////          
// squareOfWalls.cpp
//
// This program draws a square of flat walls in 3D space,
// one side of which is in outline and the other is filled.
//
// Sumanta Guha.
/////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Drawing routine.
void drawScene(void)
{  
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   
   glLoadIdentity();

   gluLookAt(0.0, 7.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Front faces filled, back faces outlined.
   glPolygonMode(GL_FRONT, GL_FILL);
   glPolygonMode(GL_BACK, GL_LINE);

   // Draw walls as a single triangle strip.
   glBegin(GL_TRIANGLE_STRIP);
      glVertex3f(-5.0, 1.0, 5.0);
      glVertex3f(-5.0, -1.0, 5.0);
      glVertex3f(5.0, 1.0, 5.0);
      glVertex3f(5.0, -1.0, 5.0);
      glVertex3f(5.0, 1.0, -5.0);
      glVertex3f(5.0, -1.0, -5.0);
      glVertex3f(-5.0, 1.0, -5.0);
      glVertex3f(-5.0, -1.0, -5.0);
      glVertex3f(-5.0, 1.0, 5.0);
      glVertex3f(-5.0, -1.0, 5.0);
   glEnd();

   glFlush();
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

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
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
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("squareOfWalls.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}
