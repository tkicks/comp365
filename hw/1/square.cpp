///////////////////////////////////
// square.cpp
//
// OpenGL program to draw a square.
//
// Sumanta Guha.
//
// 08/30/2014 - Slack-Moehrle
// - changed #ifdef for __APPLE__
// - added $ifdef _WIN32 to block out for __AAPLE__
///////////////////////////////////
///////////////////////////////////
//
//  Tyler Kickham
//  Homework 1
//
///////////////////////////////////
#include <stdlib.h>

#ifdef __APPLE__
# include <GLUT/glut.h>
# include <OpenGL/glext.h>
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
// #  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   // set color to black
   glColor3f(0.0, 0.0, 0.0);

   // Draw a polygon with specified vertices.
   glBegin(GL_POLYGON);
      glVertex3f(10.0, 10.0, 0.0);
      glVertex3f(80.0, 10.0, 0.0);
      glVertex3f(80.0, 90.0, 0.0);
      glVertex3f(10.0, 90.0, 0.0);
   glEnd();

   // set color to blue
   glColor3f(0.0, 0.0, 1.0);

   // Draw a rectangle
   glRectf(15.0, 80.0, 50.0, 65.0);

   // Set line width to 30px
   glLineWidth(30.0);

   // set color to green
   glColor3f(0.0, 1.0, 0.0);

   // Draw a rectangle
   glBegin(GL_LINE_STRIP);
      glVertex3f(75.0, 80.0, 0.0);
      glVertex3f(75.0, 50.0, 0.0);
      glVertex3f(55.0, 50.0, 0.0);
      glVertex3f(55.0, 80.0, 0.0);
      glVertex3f(75.0, 80.0, 0.0);
   glEnd();

   // set color to red
   glColor3f(1.0, 0.0, 0.0);

   // Set line width to 500px
   glLineWidth(500.0);

   // Draw rectangle
   glBegin(GL_LINES);
      glVertex3f(15.0, 40.0, 0.0);
      glVertex3f(75.0, 40.0, 0.0);
   glEnd();

   // set color to cyan
   glColor3f(0.0, 1.0, 1.0);

   // Draw square on right edge
   glBegin(GL_POLYGON);
      glVertex3f(85.0, 70.0, 0.0);
      glVertex3f(85.0, 30.0, 0.0);
      glVertex3f(115.0, 30.0, 0.0);
      glVertex3f(115.0, 70.0, 0.0);
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
   glViewport(0, 0, w, h);
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

#ifdef _WIN32
  glutInitContextVersion(4, 3);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
#endif

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("My Rectangles");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

#ifdef _WIN32
   glewExperimental = GL_TRUE;
   glewInit();
#endif

   setup();

   glutMainLoop();
}
