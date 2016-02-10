//////////////////////////////////////////////////////////////
// squareAnnulusAndTriangle.cpp
//
// This program draws a square annulus and a triangle -
// it illustrates the use of multiple vertex/color arrays.
//
// Interaction:
// Press the space bar to toggle between wireframe and filled.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Begin globals.
static int isWire = 0; // Is wireframe?

// Vertex co-ordinate vectors for the annulus.
static float vertices1[] =  
{	
	30.0, 30.0, 0.0,
    10.0, 10.0, 0.0, 
    70.0, 30.0, 0.0,
    90.0, 10.0, 0.0,
    70.0, 70.0, 0.0,
    90.0, 90.0, 0.0,
	30.0, 70.0, 0.0,
	10.0, 90.0, 0.0
};

// Vertex color vectors for the annulus.
static float colors1[] =  
{
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 0.0, 1.0,
	0.0, 1.0, 1.0,
	1.0, 0.0, 0.0
};

// Intertwined array of vertex co-ordinate and color vectors for the triangle.
static float vertices2AndColors2Intertwined[] =
{
	40.0, 40.0, 0.0, 0.0, 1.0, 1.0,
    60.0, 40.0, 0.0, 1.0, 0.0, 0.0,
    60.0, 60.0, 0.0, 0.0, 1.0, 0.0
};
// End globals.

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);  

   if (isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // The vertex and color array pointers point to the data for the annulus.
   glVertexPointer(3, GL_FLOAT, 0, vertices1);
   glColorPointer(3, GL_FLOAT, 0, colors1);

   // Draw square annulus.
   glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i < 10; ++i) glArrayElement(i%8);
   glEnd();

   // The vertex and color array pointers point to the intertwined data for the triangle.
   // The value of the stride parameter is as specified because there are six 
   // floating point values in the intertwined array between the start of 
   // successive co-ordinate or color vectors.
   glVertexPointer(3, GL_FLOAT, 6*sizeof(float), &vertices2AndColors2Intertwined[0]);
   glColorPointer(3, GL_FLOAT, 6*sizeof(float), &vertices2AndColors2Intertwined[3]);

   // Draw triangle.
   glBegin(GL_TRIANGLES);
      for(i = 0; i < 3; ++i) glArrayElement(i);
   glEnd();

   glFlush();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
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
   switch(key) 
   {
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
   glutCreateWindow("squareAnnulusAndTriangle.cpp");
   setup();
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop();

   return 0;   
}
