/* mytexture.cpp
 *
 * Texture mapping starting from perspect2.cpp
 * Draws a texture onto a parallelpiped.
 *
 */

#include <iostream>
#include <GL/glut.h>
#include "glm.h"	// find in /tutors

void rectSolid ();
void mymenu (int);

using namespace std;

float xPos = 0.0;   // x position of camera
float yPos = 0.0;   // y position of camera
float zPos = 3.0;   // distance the center of projection from frustum 
int wireframe = 0;  // toggle for wire-frame
int depthbuff = 1;  // toggle for depth buffer
int texture = 1;    // toggle for texture
unsigned char* image = NULL;  // name of texture map
int texWidth;
int texHeight;

void display ()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();

   // Change position of camera in x, y, and z
   gluLookAt (xPos, yPos, zPos, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // draw solid object
   rectSolid ();

   glutSwapBuffers();
}

// Draw a rectangular solid centered at 0, 0, 0
void rectSolid ()
{
   float Xr = 0.5;	// right x
   float Xl = -0.5;	// left x
   float Yt = 0.5;	// top y
   float Yb = -0.5;	// bottom y
   float Zf = 0.8;	// front z
   float Zb = -0.8;	// back z

   glColor3f (0.2, 0.0, 1.0);

   if (texture)
      glEnable (GL_TEXTURE_2D);

   // right side (defined in counter-clockwise order)
   glBegin (GL_QUADS);
      // must define texture BEFORE each vertex!!
      // Textures are defined 0,0 for bottom left
      glTexCoord2f (0, 0);
      glVertex3f (Xr, Yb, Zf);  // lower left

      glTexCoord2f (1, 0);
      glVertex3f (Xr, Yb, Zb);  // lower right

      glTexCoord2f (1, 1);
      glVertex3f (Xr, Yt, Zb);  // upper right

      glTexCoord2f (0, 1);
      glVertex3f (Xr, Yt, Zf);  // upper left
   glEnd();
   if (texture)
      glDisable (GL_TEXTURE_2D);

   // top
   glBegin (GL_QUADS);
      glVertex3f (Xr, Yt, Zf);
      glVertex3f (Xr, Yt, Zb);
      glVertex3f (Xl, Yt, Zb);
      glVertex3f (Xl, Yt, Zf);
   glEnd();

   // left side
   glBegin (GL_QUADS);
      glVertex3f (Xl, Yt, Zb);
      glVertex3f (Xl, Yt, Zf);
      glVertex3f (Xl, Yb, Zf);
      glVertex3f (Xl, Yb, Zb);
   glEnd();

   // bottom
   glBegin (GL_QUADS);
      glVertex3f (Xr, Yb, Zf);
      glVertex3f (Xl, Yb, Zf);
      glVertex3f (Xl, Yb, Zb);
      glVertex3f (Xr, Yb, Zb);
   glEnd();

   // back
   glColor3f (1.0, 0.5, 0.5);
   glBegin (GL_QUADS);
      glVertex3f (Xr, Yt, Zb); 
      glVertex3f (Xr, Yb, Zb); 
      glVertex3f (Xl, Yb, Zb); 
      glVertex3f (Xl, Yt, Zb);
   glEnd();

   if (texture)
      glEnable (GL_TEXTURE_2D);

   // front
   glColor3f (0.0, 0.5, 1.0);
   glBegin (GL_QUADS);
      glTexCoord2f (0, 0);
      glVertex3f (Xl, Yb, Zf);  // lower left

      glTexCoord2f (1, 0);
      glVertex3f (Xr, Yb, Zf);  // lower right

      glTexCoord2f (1, 1);
      glVertex3f (Xr, Yt, Zf);  // upper right

      glTexCoord2f (0, 1);
      glVertex3f (Xl, Yt, Zf);  // upper left
   glEnd();

/* ----
   // an extra one for kicks (equilateral triangle, point down)
   // This tests how to map texture onto a non-rectangular polygon
   glBegin (GL_POLYGON);
      glTexCoord2f (0.5, 1.0);
      glVertex3f (0.0, Yb, 1.0); 	// bottom center of triangle

      glTexCoord2f (1.0, 0.0);
      glVertex3f (Xr, Yt, 1.0);		// upper right corner

      glTexCoord2f (0.0, 0.0);
      glVertex3f (Xl, Yt, 1.0);		// upper left corner
   glEnd();
 --- */

   if (texture)
      glDisable (GL_TEXTURE_2D);
} // rectSolid ()
   

void init ()
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable (GL_DEPTH_TEST);
   // get texture map as a ppm file; width = 2^m, height = 2^n
   // glmReadPPM is in glm.h
   image=glmReadPPM ("CelebDMud.ppm", &texWidth, &texHeight);
   //cout << texWidth << " " << texHeight << endl;

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
   glTexImage2D (GL_TEXTURE_2D, 	// target: 2D texture
		   0,			// level = 0 unless multiple resolutions
		   GL_RGB,		// internal image format 
		                        // (see OpenGL text for options)
		   texWidth,		// image width
		   texHeight,		// image height
		   0,			// border width (0 or 1;see OpenGL text)
		   GL_RGB,		// image format (see OpenGL text)
		   GL_UNSIGNED_BYTE,  	// format of data within image file
		   image);		// image file

   /* ----
   // alternatively, store the image as a mipmap 
   gluBuild2DMipmaps (GL_TEXTURE_2D,    // target: 2D texture
		      3,                // 3 components (R, G, and B)
		      texWidth,         // image width
		      texHeight,        // image height
		      GL_RGB,           // image format
		      GL_UNSIGNED_BYTE, // format of data within image file
		      image);		// image file
   ---- */

}  // init ()

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1, 1, -1, 1, 2.0, 25.0);
   glMatrixMode (GL_MODELVIEW);
   glutSwapBuffers();
}
void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case '-': zPos = zPos + 1;
                break;
      case '+': zPos = zPos - 1;
                break;
      case '4': xPos = xPos - 1;
                break;
      case '6': xPos = xPos + 1;
                break;
      case '8': yPos = yPos + 1;
                break;
      case '2': yPos = yPos - 1;
                break;
      case 'q': exit (1);
   }
   glutPostRedisplay ();
}

void initmenu ()
{
   glutCreateMenu (mymenu);
   glutAddMenuEntry ("Menu", 0);
   glutAddMenuEntry ("", 0);
   glutAddMenuEntry ("Toggle Wire-Frame", 1);
   glutAddMenuEntry ("Toggle Depth-Buffer", 2);
   glutAddMenuEntry ("Toggle Texture", 3);
   glutAddMenuEntry ("Quit", 6);
   glutAttachMenu (GLUT_LEFT_BUTTON);
   glutAttachMenu (GLUT_RIGHT_BUTTON);
}

void mymenu (int value)
// Define actions for menu options
{
   switch (value) {
      // toggle wire-frame; do both front- and rear-facing polygons
      case 1 : if (wireframe) {
		  glPolygonMode (GL_FRONT, GL_FILL);
		  glPolygonMode (GL_BACK, GL_FILL);
		  wireframe--;
	       } else {
		  glPolygonMode (GL_FRONT, GL_LINE);
		  glPolygonMode (GL_BACK, GL_LINE);
		  wireframe++;
	       }
	       break;
      // D  Toggle hidden-surface removal (z-buffer test)
      case 2 : if (depthbuff) {
		  glDisable (GL_DEPTH_TEST);
		  depthbuff--;
	       } else {
		  glEnable (GL_DEPTH_TEST);
		  depthbuff++;
	       }
	       break;
      case 3 : if (texture) 
	          texture--;
	       else
		  texture++;
	       break;
      case 6 : exit (1);
   }
   glutPostRedisplay();
} // mymenu ()


int main (int argc, char** argv) {
   glutInit(&argc, argv);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

   glutInitWindowSize (500, 500); 

   glutInitWindowPosition (100, 100);

   glutCreateWindow ("Texture");

   init ();

   initmenu ();

   glutDisplayFunc(display); 

   glutReshapeFunc (reshape);

   glutKeyboardFunc (keyboard);

   glutMainLoop();

   return 0; 
}
