/////////////////////////////////////////////////////////////////////
// bezierCurveWithEvalCoord.cpp
//  
// This program draws a Bezier curve of order 6 using glEvalCoord1().
//
// Sumanta Guha
/////////////////////////////////////////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Begin globals.
// Control points.
static float controlPoints[6][3] = 
{
	{ -4.0, -2.0, 0.0}, { -3.0, 2.0, -5.0}, { -1.0, -1.0, 2.0},
	{0.0, 2.0, -2.0}, {3.0, -3.0, 1.0}, { 4.0, 0.0, -1.0}
};
// End globals.

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Specify and enable the Bezier curve.
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 6, controlPoints[0]);
   glEnable(GL_MAP1_VERTEX_3);
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 0.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   
   // Draw the Bezier curve by approximating with a line strip.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 50; i++) glEvalCoord1f( (float)i/50.0 );
   glEnd();

   // Draw the control points as dots.
   glPointSize(5.0);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
   for (i = 0; i < 6; i++) 
      glVertex3fv(controlPoints[i]);
   glEnd();

   glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
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
   glutCreateWindow("bezierCurveWithEvalCoord.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}

