/*
 *  Michael Gousie
 *  1/10/11
 *
 *  displayList.cpp
 *
 *  Program to use a display list.
 *  Shows the 6 steps to make and display such a list.
 *
 */

using namespace std;

#include <iostream>
#include <GL/glut.h>

const int DECK = 1;

class bridge {
public:
   GLuint deckList;		// 1. make a variable to hold the list
    
public:
   bridge () {};
   void makeBridge ();
};

bridge myBridge;

/*****************************************************/

void bridge :: makeBridge () {
// method to set up lists defining the bridge
   int i;
   int side;

   // 2.  generate a list
   deckList = glGenLists (DECK);

   // 3.  compile the new list
   glNewList (deckList, GL_COMPILE);

      // set up some colors
      for (i = 1; i < 6; i++) {
         if (i % 2 == 0)
            glColor3f (0, 0, 1);
         else
            glColor3f (1, 0, 0);

         // define in counter-clockwise fashion from outside looking at polygon
         glBegin (GL_QUADS);
            side = i * 30;
            glVertex3i (side, 100, 67);
            glVertex3i (side+30, 100, 67);
            glVertex3i (side+30, 127, 67);
            glVertex3i (side, 127, 67);
         glEnd();
      }

   // 4.  end the list
   glEndList();
}

/*****************************************************/

void display () {
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // 5.  cal the deck list
   glCallList (myBridge.deckList);

   glFlush();
}  // display

/*****************************************************/

void keyboard (unsigned char key, int x, int y) {
   switch (key) {
      case 27  :
      case 'q' : exit (1);
   }
}  // keyboard

/*****************************************************/

void init() {
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   glClearColor (0.8, 0.8, 0.8, 0.0);
   glOrtho (0.0, 200.0, 0.0, 200.0, -200.0, 200.0);

   // 6.  call method to define the bridge
   myBridge.makeBridge();
}  // init

/*****************************************************/

int main (int argc, char** argv) {
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutInitWindowSize (1000, 600);

   glutCreateWindow ("Simple display list");

   init ();

   glutKeyboardFunc (keyboard);

   glutDisplayFunc (display);

   glutMainLoop();

   return 0;
}
