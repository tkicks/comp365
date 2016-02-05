/* colorFun.cpp */

using namespace std;
#include <iostream>
#include <GL/glut.h>

float r = 1; float g = 1; float b = 1;

void display ()
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (r, g, b);
   glBegin (GL_POLYGON);
      glVertex3f (0.5, 0.75, 0.0);
      glVertex3f (0.25, 0.25, 0.0);
      glVertex3f (0.75, 0.25, 0.0);
   glEnd();
   glFlush ();
}

void init ()
{
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1); 
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   glOrtho (0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case '7' : if (r < 1)
	            r += 0.1;
		 break;
      case '1' : if (r > 0)
	            r -= 0.1;
		 break;
      case '8' : if (g < 1) g += 0.1;
                 break;
      case '2' : if (g > 0) g -= 0.1;
                 break;
      case '9' : if (b < 1) b += 0.1;
                 break;
      case '3' : if (b > 0) b -= 0.1;
                 break;
      case 27  : 
      case 'q' : exit (1);
   }
   glutPostRedisplay();
   cout << r << " " << g << " " << b << endl;
}

int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (650, 250); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Playing with colors");
   init ();
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display); 
   glutMainLoop();
   return 0; 
}