// menu.cpp
// Demo showing use of easy menus

#include <GL/glut.h>
#include <iostream>

using namespace std;

void mymenu (int);   // menu callback function


void display ()
{
   glClear (GL_COLOR_BUFFER_BIT);

   glFlush ();
}

void init ()
{
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   glClearColor (0.0, 0.0, 0.8, 0.0);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   
   gluOrtho2D (0.0, 300, 0.0, 300);
}

void initmenu ()
// set up menu
{
   glutCreateMenu (mymenu);       // register callback function
   glutAddMenuEntry ("Menu", 0);
   glutAddMenuEntry ("", 0);
   glutAddMenuEntry ("Hello", 1);
   glutAddMenuEntry ("Goodbye", 2);
   glutAttachMenu (GLUT_LEFT_BUTTON);
}

void mymenu (int value)
// Define actions for the menu based on values given in initmenu()
{
   switch (value) {
      case 1: cout << "Hey there, hello!" << endl;
              break;
      case 2: cout << "Goodbye" << endl;
              exit (1);
   }
}

int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (50, 50); 
   glutCreateWindow ("lil menu");
   init ();

   initmenu ();

   glutDisplayFunc(display); 
   glutMainLoop();

   return 0; 
}
