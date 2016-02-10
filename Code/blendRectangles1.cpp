//////////////////////////////////////////////////////////////////////////////////////          
// blendRectangles1.cpp
//
// This program draws two rectangles that are blended in their intersection. The order
// in which the rectangles are drawn can be changed.
//
// Interaction:
// Press space to toggle between either rectangle being drawn first.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals.
static int isBlueFirst = 1; // Blue rectangle drawn first?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Function to draw a blue rectangle at z=0.
void drawBlueRectangle(void)
{
   glColor4f(0.0, 0.0, 1.0, 0.5); // Translucent: alpha value = 0.5.

   glPushMatrix();
   glTranslatef(0.0, -0.5, 0.0);
   glBegin(GL_POLYGON);
      glVertex3f(-1.0, -0.25, 0.0);
	  glVertex3f(1.0, -0.25, 0.0);
      glVertex3f(1.0, 0.25, 0.0);
	  glVertex3f(-1.0, 0.25, 0.0);
   glEnd();
   glPopMatrix();
}

// Function to draw a red rectangle at z=0.2.
void drawRedRectangle(void)
{
   glColor4f(1.0, 0.0, 0.0, 0.5); // Translucent: alpha value = 0.5.

   glPushMatrix();
   glTranslatef(0.5, 0.0, 0.2);
   glRotatef(90.0, 0.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
      glVertex3f(-1.0, -0.25, 0.0);
	  glVertex3f(1.0, -0.25, 0.0);
      glVertex3f(1.0, 0.25, 0.0);
	  glVertex3f(-1.0, 0.25, 0.0);
   glEnd();
   glPopMatrix();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glDisable(GL_DEPTH_TEST); // Disable depth testing.

   glEnable(GL_BLEND); // Enable blending.
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
}

// Drawing routine.
void drawScene()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   if (isBlueFirst) // Draw blue rectangle first.
   {
	  drawBlueRectangle();
      drawRedRectangle();
	  glColor3f(0.0, 0.0, 0.0);
	  glRasterPos3f(-2.5, 2.5, -2.0);
      writeBitmapString((void*)font, "Blue rectangle drawn first, red rectangle second in code!");
   }
   else // Draw red rectangle first.
   {
	  drawRedRectangle();
	  drawBlueRectangle();
	  glColor3f(0.0, 0.0, 0.0);
	  glRasterPos3f(-2.5, 2.5, -2.0);
      writeBitmapString((void*)font, "Red rectangle drawn first, blue rectangle second in code!");
   }

   glFlush();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
      case ' ':
         if (isBlueFirst) isBlueFirst = 0;
		 else isBlueFirst = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between either rectangle being drawn first." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("blendRectangles1.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMainLoop();
   
   return 0;
}
