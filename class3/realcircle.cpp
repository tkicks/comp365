// circle.cpp
// Demo to show how circles, and points can be drawn.
// Also shows use of the glOrtho function to create desired window coordinates.

using namespace std;

#include <GL/glut.h>

void display ()
{
   int i;
   /* circles are made from quadric objects */
   GLUquadricObj *myobject;   
   GLUquadricObj *myobject2;
   GLUquadricObj *myobject3;

   myobject = gluNewQuadric();
   myobject2 = gluNewQuadric();
   myobject3 = gluNewQuadric();

   /* set up the window */
   glClear (GL_COLOR_BUFFER_BIT);

   /* draw some points */
   glColor3f (1.0, 1.0, 1.0);
   glBegin (GL_POINTS);
      for (i = 10; i < 400; i+=10)
         glVertex2i (i, 200);
      for (i = 0; i < 400; i++)
         glVertex2i (i, 400-i);
   glEnd();

   /* for circles, origin is assumed (i.e., the center of the circle will */
   /* be at the origin); must translate first to proper position */
   glTranslatef (80.0, 80.0, 0.0);  

   /* draw a solid circle */
   /* circle parameters: object pointer, inner radius, outer radius, */
   /* number of slices, number of rings */
   gluDisk (myobject, 0.0, 80.0, 100, 4);

   /* translating again starts from where CP (Current Position) left off */
   glTranslatef (240.0, 0.0, 0.0);
   glColor3f (0.0, 1.0, 0.0);
   gluQuadricDrawStyle (myobject2, GLU_LINE);  /* wireframe */
   gluDisk (myobject2, 40.0, 80.0, 10, 4);
   // gluDisk (myobject2, 20.0, 50.0, 4, 2);

   glTranslatef (0.0, 240.0, 0.0);
   gluQuadricDrawStyle (myobject3, GLU_LINE);  /* wireframe */
   glColor3f (0.0, 0.0, 1.0);
   gluDisk (myobject3, 0.0, 80.0, 60, 1);

   glTranslatef (-240.0, 0.0, 0.0);
   gluQuadricDrawStyle (myobject3, GLU_LINE);  /* wireframe */
   glColor3f (0.0, 0.0, 1.0);
   gluDisk (myobject3, 0.0, 80.0, 60, 1);

   /* by translating back to the origin, the screen can be redrawn properly */
   glTranslatef (-80.0, -320.0, 0.0);

   /* flush the buffer (i.e., draw the thing!) */
   glFlush ();
}

void init ()
{
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   
   /* this time, I made the "internal" window coordinates the same */
   /* as the "external" coordinates */
   glOrtho (0.0, 400.0, 0.0, 400.0, -1.0, 1.0);
}

int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   /* make the window square so that aspect ration is correct */
   glutInitWindowSize (400, 400); 

   glutInitWindowPosition (100, 100);

   glutCreateWindow ("gee - circles.");

   init ();

   glutDisplayFunc(display); 

   glutMainLoop();

   return 0; 
}