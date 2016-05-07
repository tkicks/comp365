/////////////////////////////////////////////////////////////////////////       
// sizeNormal.cpp
//
// This program based on litTriangle.cpp adds the functionality to change
// the length (but not the direction) of one of the three normals. This
// normal is indicated by a white arrow.
//
// Interaction:
// Press up/down arrow keys to increase/decrease length of normal.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////// 

#include <iostream>
#include "glm.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals.
static float d = 1.0; // Length of normal.
static float Yangle = 0;
static bool texture = true;

unsigned char* image = NULL;
int texWidth;
int texHeight;

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   image=glmReadPPM ("CelebDMud.ppm", &texWidth, &texHeight);

   // set up texture parameters
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   // set parameters for the edges of texture
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // replace polygon with texture (not just cover polygon, in which case
   // the color of the previous polygon shows through the texture)
   glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   // store image as a texture
   glTexImage2D (GL_TEXTURE_2D,  // target: 2D texture
         0,       // level = 0 unless multiple resolutions
         GL_RGB,     // internal image format 
                              // (see OpenGL text for options)
         texWidth,      // image width
         texHeight,     // image height
         0,       // border width (0 or 1;see OpenGL text)
         GL_RGB,     // image format (see OpenGL text)
         GL_UNSIGNED_BYTE,    // format of data within image file
         image);     // image file

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);

   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightPos[] = { 0.0, 1.5, 3.0, 1.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  
   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

   // Material property vectors.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Enable color material mode:
   // The ambient and diffuse color of the front faces will track the color set by glColor()
   // (only the front face of the triangle is visible).
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
}

// Drawing routine.
void drawScene()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float Xr = 1.0;
   float Xl = -1.0;
   float Yb = -1.0;
   float Yt = 0.0;


   // textures -------------------------------------------
   if (texture)
      glEnable (GL_TEXTURE_2D);

   glBegin (GL_POLYGON);
      glTexCoord2f (0.5, 1.0);
      glVertex3f (0.0, Yt, 0.0);    // top center of triangle

      glTexCoord2f (1.0, 0.0);
      glVertex3f (Xr, Yb, 0.0);     // bottom right corner

      glTexCoord2f (0.0, 0.0);
      glVertex3f (Xl, Yb, 0.0);     // bottom left corner
   glEnd();

   if (texture)
      glDisable (GL_TEXTURE_2D);

   // end textures ---------------------------------------

   glLoadIdentity();
   gluLookAt(0.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glRotatef(Yangle, 0.0, 1.0, 0.0);

   // Rotate scene.
   glRotatef (30.0, 0.0, 1.0, 0.0);

   // Draw triangle.
   glBegin(GL_TRIANGLES);
      glNormal3f(0.0, 0.0, 1.0);
	  glColor4f(0.0, 0.0, 1.0, 1.0); // Ambient and diffuse color.
      glVertex3f(-1.0, -1.0, 0.0);
      
	  glNormal3f(0.0, 0.0, 1.0);
      glColor4f(0.0, 1.0, 0.0, 1.0); // Ambient and diffuse color.
	  glVertex3f(1.0, -1.0, 0.0);
      
	  glNormal3f(0.0, 0.0, d);
      glColor4f(1.0, 0.0, 0.0, 1.0); // Ambient and diffuse color.
	  glVertex3f(0.0, 1.0, 0.0);
   glEnd();

   glBegin(GL_POLYGON);
      glColor4f(0.0, 0.0, 1.0, 1.0);
      glVertex3f(-1.0, -1.0, 0.0);
      glColor4f(0.0, 1.0, 0.0, 1.0);
      glVertex3f(0.0, 1.0, 0.0);
      glColor4f(1.0, 0.0, 0.0, 1.0);
      glVertex3f(0.0, 0.0, -1.0);
   glEnd();

   glBegin(GL_POLYGON);
      glNormal3f(0.0, 2.0, -2.0);
      glColor4f(0.0, 0.0, 1.0, 1.0);
      glVertex3f(-1.0, -1.0, 0.0);
      
      glNormal3f(0.0, 2.0, -2.0);
      glColor4f(0.0, 1.0, 0.0, 1.0);
      glVertex3f(1.0, -1.0, 0.0);

      glNormal3f(0.0, 2.0, -2.0);
      glColor4f(1.0, 0.0, 0.0, 1.0);
      glVertex3f(0.0, 0.0, -1.0);
   glEnd();
  
   // Draw arrow indicating normal.
   glDisable(GL_LIGHTING);
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
	  glVertex3f(0.0, 1.0, 0.0);
	  glVertex3f(0.0, 1.0, d);
	  glVertex3f(0.0, 1.0, d);
	  glVertex3f(0.1, 1.0, d-0.1);
	  glVertex3f(0.0, 1.0, d);
	  glVertex3f(-0.1, 1.0, d-0.1);
   glEnd();
   glEnable(GL_LIGHTING);

   glutSwapBuffers();
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
      case 'y': Yangle += 5.0;
                if (Yangle > 360.0) Yangle -= 360.0;
                glutPostRedisplay();
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_UP) d += 0.1;
   if (key == GLUT_KEY_DOWN)
   {
      if (d > 0.0) d -= 0.1;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << " Press up/down arrow keys to increase/decrease length of normal." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("sizeNormal.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop();
   
   return 0;
}
