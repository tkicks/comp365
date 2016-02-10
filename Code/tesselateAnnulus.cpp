///////////////////////////////////////////////////////////////////////////////////
// tesselateAnnulus.cpp
//
// This program tesselates a square annulus, a non-convex polygon with two boundary 
// components, using GLU tesselation routines.
//
// Interaction:
// Press the space bar to toggle between wireframe and filled.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Begin globals.
static int isWire = 0; // Is wireframe?

// Annulus outside boundary vertices.   
static double outsideBoundary[4][3] =
{
	{ 10.0, 10.0, 0.0 },
	{ 90.0, 10.0, 0.0 },
	{ 90.0, 90.0, 0.0 },
	{ 10.0, 90.0, 0.0 }
};

// Annulus inside boundary vertices.  
static double insideBoundary[4][3] =
{
	{ 30.0, 30.0, 0.0 },
	{ 70.0, 30.0, 0.0 },
	{ 70.0, 70.0, 0.0 },
	{ 30.0, 70.0, 0.0 }
};

static GLUtesselator *tobj; // Create a pointer to a tesselation object.
// End globals.

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(0.0, 0.0, 0.0);
          
   int i;

   tobj = gluNewTess(); // Create the tesselation object.

   // Define the call back functions to be used.
   
  #ifdef WIN32
      gluTessCallback(tobj, GLU_TESS_BEGIN, (void(__stdcall *)())glBegin);
      gluTessCallback(tobj, GLU_TESS_VERTEX, (void(__stdcall *)())glVertex3dv);
      gluTessCallback(tobj, GLU_TESS_END, (void(__stdcall *)())glEnd);
   #else
      gluTessCallback(tobj, GLU_TESS_BEGIN, (void(*)())glBegin);
      gluTessCallback(tobj, GLU_TESS_VERTEX, (void(*)())glVertex3dv);
      gluTessCallback(tobj, GLU_TESS_END, (void(*)())glEnd);
   #endif
   
   if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   gluTessBeginPolygon(tobj, (GLvoid *)0); // Begin specifying the square annulus.

   gluTessBeginContour(tobj); // Begin specifying outside contour.
      for(i = 0; i < 4; ++i) gluTessVertex(tobj, outsideBoundary[i], outsideBoundary[i]);
   gluTessEndContour(tobj); // End specifying outside contour.

   gluTessBeginContour(tobj); // Begin specifying inside contour.
      for(i = 0; i < 4; ++i) gluTessVertex(tobj, insideBoundary[i], insideBoundary[i]);
   gluTessEndContour(tobj); // End specifying inside contour.

   gluTessEndPolygon(tobj); // End specifying the square annulus.

   gluDeleteTess(tobj); // Delete tesselation object.

   glPopMatrix();

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
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) {
      case ' ':
         if (isWire == 0) isWire = 1;
         else isWire = 0;
         glutPostRedisplay();
         break;          
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space bar to toggle between wireframe and filled." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("tesselateAnnulus.cpp");
   setup();
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop();

   return 0;   
}
