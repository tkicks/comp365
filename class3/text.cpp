/* text.cpp
 * Small text demo, using pre-defined bitmaps/stroke characters.
 * One can also create one's own fonts by defining bitmaps.
 * This is a lengthy process.  Check the example /samples/bitmap1.c
 * for more information.
 */

#include <iostream>
#include <GL/glut.h>
using namespace std;

GLenum rgb;  /* lovely global used by tkmap.c (which is why the include
                is below this line) */

#include "tkmap.c"

void display ()
{
   string myStr = "This makes it better!\0";
   int i;

   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 0.0);
   glRasterPos2i (100, 100);

   /* more fonts and sizes can be found in /GL/glut.h */
   glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, 'F');
   glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, 'u');
   glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, 'n');

   glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, 'f');
   glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, 'u');
   glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, 'n');

   glRasterPos2i (50, 50);
   i = 0;
   while (myStr [i] != '\0') {
      glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, myStr [i]);
      i++;
   }
   
   glColor3f (1.0, 0.0, 1.0);

   glFlush ();
}

void init ()
{
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glOrtho (0.0, 400.0, 0.0, 400.0, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
   /* Draw characters at x, y position
    * Note all the nice numeric constants I have in this function; 
    * what great programming!
    */

   /* x, y is cursor position in window with 0, 0 at UPPER LEFT */
   cout << "x, y: " << x << ", " << y << "; key: " << key << endl;

   if (key == 27)
      exit (1);

   /* draw lower-case letters */
   else if (key >= 97) {
      /* Stroke characters are made by drawing line segments.
       * Thus, in order to draw them in various places, each line
       * segment must be translated, just like polygons.
       * Furthermore, the x is automatically incremented by the size
       * of the character.  To translate back to the proper x, y position
       * requires negating this extra increment in x.
       *
       * Check out font.c in /samples which shows how one can manipulate
       * these characters.
       */
      glTranslatef (x, 400-y, 0);
      glutStrokeCharacter(GLUT_STROKE_ROMAN, key);
      glTranslatef (-(x+68), y-400, 0);
   }

   /* draw upper case letters */
   else {
      /* Bitmap characters are an MxN matrix of points.  One must
       * position the matrix in the proper spot before drawing.  To do 
       * this, use the "move" command which in OpenGL is glRasterPos.
       */
      glRasterPos2i (x, 400-y);
      glutBitmapCharacter (GLUT_BITMAP_9_BY_15, key);
   }
   glFlush();
}


int main (int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutInitWindowSize (400, 400);

   /* put the window at 100, 100 on user's screen */
   glutInitWindowPosition (100, 100);

   glutCreateWindow ("Text Demo");

   init ();

   glutKeyboardFunc(keyboard);

   glutDisplayFunc(display); 

   glutMainLoop();

   return 0; 
}