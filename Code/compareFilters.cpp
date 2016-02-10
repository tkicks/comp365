////////////////////////////////////////////////////////////////////////////////
// compareFilters.cpp
//
// This program shows two squares with identical images as textures that can be 
// filtered separately, and moved together.
//
// NOTE: The Textures folder must be in the same one as this program.
//
// Interaction:
// Press the up and down arrow keys to move the squares.
// Press the left arrow key to cycle through the filters for the left square.
// Press the right arrow key to cycle through the filters for the right square.
//
// Sumanta Guha
////////////////////////////////////////////////////////////////////////////////
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

using namespace std;

// Globals.
static unsigned int texture[12]; // Array of texture indices.
static float d = 0.0; // Distance parameter in gluLookAt().
static int filter1 = 0; // Filter id.
static int filter2 = 6; // Filter id.
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

// Load external texture.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[1];
   
   // Load the texture.
   image[0] = getBMPData("Textures/launch.bmp"); 

   // Bind image to texture index[0] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
    	        GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[1] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
    	        GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[2] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[3] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[4] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[4]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[5] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[5]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[6] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[6]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
    	        GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[7] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[7]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
    	        GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[8] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[8]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[9] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[9]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[10] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[10]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind image to texture index[11] with specified mag and min filters.
   glBindTexture(GL_TEXTURE_2D, texture[11]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[0]->sizeX, image[0]->sizeY, 
	                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Write messages.
void writeData(void)
{
   switch(filter1) 
   {
      case 0:
         glRasterPos3f(-10.0, 4.9, -5.1); writeBitmapString((void*)font, "Left mag filter: GL_NEAREST");
         glRasterPos3f(-10.0, 4.6, -5.1); writeBitmapString((void*)font, "Left min filter: GL_NEAREST");
         break;
      case 1:
         glRasterPos3f(-10.0, 4.9, -5.1); writeBitmapString((void*)font, "Left mag filter: GL_LINEAR");
         glRasterPos3f(-10.0, 4.6, -5.1); writeBitmapString((void*)font, "Left min filter: GL_LINEAR");
         break;
      case 2:
         glRasterPos3f(-10.0, 4.9, -5.1); writeBitmapString((void*)font, "Left mag filter: GL_LINEAR");
         glRasterPos3f(-10.0, 4.6, -5.1); writeBitmapString((void*)font, "Left min filter: GL_NEAREST_MIPMAP_NEAREST");
         break;
      case 3:
         glRasterPos3f(-10.0, 4.9, -5.1); writeBitmapString((void*)font, "Left mag filter: GL_LINEAR");
         glRasterPos3f(-10.0, 4.6, -5.1); writeBitmapString((void*)font, "Left min filter: GL_LINEAR_MIPMAP_NEAREST");
         break;
      case 4:
         glRasterPos3f(-10.0, 4.9, -5.1); writeBitmapString((void*)font, "Left mag filter: GL_LINEAR");
         glRasterPos3f(-10.0, 4.6, -5.1); writeBitmapString((void*)font, "Left min filter: GL_NEAREST_MIPMAP_LINEAR");
         break;
      case 5:
         glRasterPos3f(-10.0, 4.9, -5.1); writeBitmapString((void*)font, "Left mag filter: GL_LINEAR");
         glRasterPos3f(-10.0, 4.6, -5.1); writeBitmapString((void*)font, "Left min filter: GL_LINEAR_MIPMAP_LINEAR");
         break;
      default:
         break;
   }

   switch(filter2) 
   {
      case 6:
         glRasterPos3f(3.1, 4.9, -5.1); writeBitmapString((void*)font, "Right mag filter: GL_NEAREST");
         glRasterPos3f(3.1, 4.6, -5.1); writeBitmapString((void*)font, "Right min filter: GL_NEAREST");
         break;
      case 7:
         glRasterPos3f(3.1, 4.9, -5.1); writeBitmapString((void*)font, "Right mag filter: GL_LINEAR");
         glRasterPos3f(3.1, 4.6, -5.1); writeBitmapString((void*)font, "Right min filter: GL_LINEAR");
         break;
      case 8:
         glRasterPos3f(3.1, 4.9, -5.1); writeBitmapString((void*)font, "Right mag filter: GL_LINEAR");
         glRasterPos3f(3.1, 4.6, -5.1); writeBitmapString((void*)font, "Right min filter: GL_NEAREST_MIPMAP_NEAREST");
         break;
      case 9:
         glRasterPos3f(3.1, 4.9, -5.1); writeBitmapString((void*)font, "Right mag filter: GL_LINEAR");
         glRasterPos3f(3.1, 4.6, -5.1); writeBitmapString((void*)font, "Right min filter: GL_LINEAR_MIPMAP_NEAREST");
         break;
      case 10:
         glRasterPos3f(3.1, 4.9, -5.1); writeBitmapString((void*)font, "Right mag filter: GL_LINEAR");
         glRasterPos3f(3.1, 4.6, -5.1); writeBitmapString((void*)font, "Right min filter: GL_NEAREST_MIPMAP_LINEAR");
         break;
      case 11:
         glRasterPos3f(3.1, 4.9, -5.1); writeBitmapString((void*)font, "Right mag filter: GL_LINEAR");
         glRasterPos3f(3.1, 4.6, -5.1); writeBitmapString((void*)font, "Right min filter: GL_LINEAR_MIPMAP_LINEAR");
         break;
      default:
         break;
   }
}

// Initialization routine.
void setup(void)
{    
   glClearColor(1.0, 1.0, 1.0, 0.0);

   // Create texture index array.
   glGenTextures(12,texture); 

   // Load external texture and generate and load procedural texture.
   loadExternalTextures();

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D);

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 0.0, 11.5 + d, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

   // Map one texture onto a square polygon.
   glBindTexture(GL_TEXTURE_2D, texture[filter1]); 
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-21.0, -10.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -10.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 10.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-21.0, 10.0, 0.0);
   glEnd();

   // Map another texture onto an adjacent square polygon.
   glBindTexture(GL_TEXTURE_2D, texture[filter2]); 
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -10.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(21.0, -10.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(21.0, 10.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 10.0, 0.0);
   glEnd();

   // Write data.
   glLoadIdentity();
   glColor3f(0.0, 0.0, 0.0);
   writeData();

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-10.0, 10.0, -5.0, 5.0, 5.0, 500.0);
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

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) 
   {
      if (d > 0.0) d -= 0.1;
   }
   if (key == GLUT_KEY_UP) 
   {
      d += 0.1;
   }
   if (key == GLUT_KEY_LEFT)
   {
      filter1++;
	  if (filter1 == 6) filter1 = 0;
   }
   if (key == GLUT_KEY_RIGHT)
   {
      filter2++;
	  if (filter2 == 12) filter2 = 6;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up and down arrow keys to move the squares." << endl
	    << "Press the left arrow key to cycle through the filters for the left square." << endl
        << "Press the right arrow key to cycle through the filters for the right square." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(1000, 500);
   glutInitWindowPosition(5, 5);
   glutCreateWindow("compareFilters.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop();

   return 0; 
}
