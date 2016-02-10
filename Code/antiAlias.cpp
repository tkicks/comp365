////////////////////////////////////////////////////////////////////////////////////////////          
// antiAlias.cpp
//
// This program show the effect of antialiasing by drawing a straight line segment which can
// be rotated and whose thickness can be changed. Antialiasing can be turned on or off.
//
// Interaction: 
// Press space to toggle between antialiasing on and off.
// Press the arrow keys to turn the line segment. 
// Press the page up/down arrow to change the line thickness.
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals.
static float angle = 0.0; // Angle to turn box.
static float width = 1.0; // Thickness of straight lines.
static int isAntialiased = 0; // Is antialiasing on?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   glLoadIdentity(); 

   // Antialiasing control.
   if (isAntialiased) 
   {
      glEnable(GL_LINE_SMOOTH); 
	  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Ask for best line antialiasing.
	  glRasterPos3f(-4.5, 4.5, -5.1); 
	  writeBitmapString((void*)font, "Antialiasing on!");
   }
   else 
   {
      glDisable(GL_LINE_SMOOTH); 
	  glRasterPos3f(-4.5, 4.5, -5.1);  
	  writeBitmapString((void*)font, "Antialiasing off!");
   }

   // Line thickness control
   glLineWidth(width);

   glTranslatef(0.0, 0.0, -15.0); 
   glRotatef(angle, 0.0, 0.0, 1.0);

   // Draw a line segment.
   glBegin(GL_LINES);
      glVertex3f(-10.0, 0.0, 0.0);
      glVertex3f(10.0, 0.0, 0.0);
   glEnd();

   glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   glEnable(GL_BLEND); // Enable blending.
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
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
         if (isAntialiased) isAntialiased = 0;
		 else isAntialiased = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_LEFT || key == GLUT_KEY_UP) angle += 1.0;
   if (key == GLUT_KEY_RIGHT || key == GLUT_KEY_DOWN) angle -= 1.0;

   if (angle > 360.0) angle -= 360.0;
   if (angle < 0.0) angle += 360.0;

   if (key == GLUT_KEY_PAGE_UP) width += 1.0;
   if (key == GLUT_KEY_PAGE_DOWN) width -= 1.0;

   if (width > 10.0) width = 10.0;
   if (width < 1.0) width = 1.0;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between antialiasing on and off." << endl
	    << "Press the arrow keys to turn the line segment." << endl 
        << "Press the page up/down arrow to change the line thickness." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("antiAlias.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop(); 

   return 0;  
}

