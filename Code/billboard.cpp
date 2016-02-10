//////////////////////////////////////////////////////////////////////////////////////////
// billboard.cpp
//
// This program illustrates billboarding. A viewpoint can move through a scene with a two
// trees textured onto a rectangle. There is the option to turn billboarding on and off.
//
// NOTE: The Textures folder must be in the same one as this program.
//
// Interaction:
// Press the space key to toggle between billboarding on and off.
// Press the up and down arrow keys to move the viewpoint.
// 
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////
// TEXTURE GREDITS:
// canLabel.bmp, thanks anonymous.
// canTop.bmp, thanks www.acoustica.com.
// cray2.bmp, thanks NASA website www.nasa.gov.
// grass.bmp, thanks www.amazingtextures.com.
// launch.bmp, thanks NASA website www.nasa.gov.
// nightsky.bmp, thanks anonymous.
// sky.bmp, thanks www.mega-tex.nl.
// trees.bmp, thanks anonymous.
////////////////////////////////////////////////

#include <cmath>					
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glut.h>
#  include <GL/glext.h>
#endif

#define PI 3.14159265358979324

using namespace std;

// Globals.
static unsigned int texture[1]; // Array of texture indices.
static float d = 40.0; // Distance of the trees image parallel to the line of sight.
static float b = 20.0; // Displacement of the trees image left of line of sight.
static int isBillboard = 0; // Is billboarding on?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Struct of bitmap file.
struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename)
{
   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;
  
   // Read input file name.
   ifstream infile(filename.c_str(), ios::binary);
 
   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *) &offset, 4); 
   
   // Get the header size of the bitmap.
   infile.read((char *) &headerSize,4);

   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read( (char *) &bmp->sizeX, 4);
   infile.read( (char *) &bmp->sizeY, 4);

   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];

   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *) bmp->data , size);
   
   // Reverse color from bgr to rgb.
   int temp;
   for (int i = 0; i < size; i += 3)
   { 
      temp = bmp->data[i];
	  bmp->data[i] = bmp->data[i+2];
	  bmp->data[i+2] = temp;
   }

   return bmp;
}

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[1]; 
   
   // Load the texture.
   image[0] = getBMPData("Textures/trees.bmp");

   // Bind trees image to texture index[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR); 
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);				
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Initialization routine.
void setup(void)
{    
   glClearColor (1.0, 1.0, 1.0, 0.0);
   glEnable(GL_DEPTH_TEST);

   // Create texture index array and load external textures.
   glGenTextures(1, texture);
   loadExternalTextures();

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D); 

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glLoadIdentity();

   // Write message.
   glDisable(GL_TEXTURE_2D);
   glPushMatrix();
   glColor3f(0.0, 0.0, 0.0);
   glRasterPos3f(-1.0, 4.0, -5.1); 
   if (isBillboard) writeBitmapString((void*)font, "Billboarding on!");
   else writeBitmapString((void*)font, "Billboarding off!");
   glPopMatrix();
   glEnable(GL_TEXTURE_2D);
   
   // Begin place the trees image.
   glPushMatrix(); 
   glTranslatef(-b, 0.0, -d);

   // If billboarding on, rotate the trees image so that it is normal to the viewing direction.
   if (isBillboard) glRotatef( atan(b/d)*(180.0/PI), 0.0, 1.0, 0.0);

   // Map the trees texture onto a vertical rectangle.
   glBindTexture(GL_TEXTURE_2D, texture[0]);  
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-2.5, -5.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(2.5, -5.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(2.5, 5.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-2.5, 5.0, 0.0);
   glEnd();

   glPopMatrix(); 
   // End place the trees image.

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-10.0, 10.0, -5.0, 5.0, 5.0, 100.0);
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
      case ' ':
         if (isBillboard) isBillboard = 0;
		 else isBillboard = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN)  d += 0.2;
   if (key == GLUT_KEY_UP) d -= 0.2;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space key to toggle between billboarding on and off." << endl
	    << "Press the up and down arrow keys to move the viewpoint." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(10, 10);
   glutCreateWindow("billboard.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop();

   return 0; 
}
