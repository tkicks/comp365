//////////////////////////////////////////////////////////////////////////////////
// mipmapLevels.cpp
//
// This program shows a square with textured with mipmaps of different colors.
// The square can be moved to see mipmaps of different levels as they are applied.
//
// Interaction:
// Press the up and down arrow keys to move the square.
//
// Sumanta Guha
//////////////////////////////////////////////////////////////////////////////////

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
static unsigned int texture[1]; // Array of texture indices.
static float d = 0.0; // Distance parameter in gluLookAt().
static unsigned char mipmapRes64[64][64][3]; // Storage for mipmap.
static unsigned char mipmapRes32[32][32][3]; // Storage for mipmap.
static unsigned char mipmapRes16[16][16][3]; // Storage for mipmap.
static unsigned char mipmapRes8[8][8][3]; // Storage for mipmap.
static unsigned char mipmapRes4[4][4][3]; // Storage for mipmap.
static unsigned char mipmapRes2[2][2][3]; // Storage for mipmap.
static unsigned char mipmapRes1[1][1][3]; // Storage for mipmap.

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

// Create mipmap images that are colored squares starting from 64 x 64 down to 1 x 1.
void createMipmaps(void)
{
   int i, j;

   for (i = 0; i < 64; i++) 
      for (j = 0; j < 64; j++) 
		 {
            mipmapRes64[i][j][0] = 0x00;
            mipmapRes64[i][j][1] = 0x00;
            mipmapRes64[i][j][2] = 0xFF;
		 }

   for (i = 0; i < 32; i++) 
      for (j = 0; j < 32; j++) 
		 {
            mipmapRes32[i][j][0] = 0x00;
            mipmapRes32[i][j][1] = 0xFF;
            mipmapRes32[i][j][2] = 0xFF;
		 }

   for (i = 0; i < 16; i++) 
      for (j = 0; j < 16; j++) 
		 {
            mipmapRes16[i][j][0] = 0xFF;
            mipmapRes16[i][j][1] = 0x00;
            mipmapRes16[i][j][2] = 0xFF;
		 }

   for (i = 0; i < 8; i++) 
      for (j = 0; j < 8; j++) 
		 {
            mipmapRes8[i][j][0] = 0xFF;
            mipmapRes8[i][j][1] = 0xFF;
            mipmapRes8[i][j][2] = 0x00;
		 }

   for (i = 0; i < 4; i++) 
      for (j = 0; j < 4; j++) 
		 {
            mipmapRes4[i][j][0] = 0xFF;
            mipmapRes4[i][j][1] = 0x00;
            mipmapRes4[i][j][2] = 0x00;
		 }

   for (i = 0; i < 2; i++) 
      for (j = 0; j < 2; j++) 
		 {
            mipmapRes2[i][j][0] = 0x00;
            mipmapRes2[i][j][1] = 0xFF;
            mipmapRes2[i][j][2] = 0x00;
		 }

   for (i = 0; i < 1; i++) 
      for (j = 0; j < 1; j++) 
		 {
            mipmapRes1[i][j][0] = 0x00;
            mipmapRes1[i][j][1] = 0x00;
            mipmapRes1[i][j][2] = 0x00;
		 }
}

// Routine to load mipmaps. 
void loadMipmaps()			
{
   // Activate texture index texture. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 

   // Set texture parameters.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify mipmaps to be bound with the currently active texture index.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, mipmapRes64);
   glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, mipmapRes32);
   glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, mipmapRes16);
   glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, mipmapRes8);
   glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, mipmapRes4);
   glTexImage2D(GL_TEXTURE_2D, 5, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, mipmapRes2);
   glTexImage2D(GL_TEXTURE_2D, 6, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, mipmapRes1);
}

// Initialization routine.
void setup(void)
{    
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Create texture index array.
   glGenTextures(1, texture); 

   // Create and load mipmaps.
   createMipmaps();
   loadMipmaps();

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D); 

  // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 0.0, 6.0 + d, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Map texture onto a square polygon.
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 0.0);
   glEnd();

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 2000.0);
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
      if (d > 0.0) d -= 0.2;
   }
   if (key == GLUT_KEY_UP) 
   {
      d += 0.2;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up and down arrow keys to move the square." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(5, 5);
   glutCreateWindow("mipmapLevels.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop();

   return 0; 
}
