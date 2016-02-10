///////////////////////////////////////////////////////////////////////////////////////////////////////
// loadTextures.cpp
//
// This stripped-down program shows how to load both external and program-generated images as textures.
//
// NOTE: The Textures folder must be in the same one as this program.
//
// Interaction:
// Press the left and right arrow keys to rotate the square.
// Press space to toggle between textures.
// Press delete to reset.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////////////////////////////////
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
static unsigned int texture[2]; // Array of texture indices.
static unsigned char chessboard[64][64][3]; // Storage for chessboard image.
static float angle = 0.0; // Angle to rotate textured square.
static int id = 0; // Currently displayed texture id.

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
   image[0] = getBMPData("Textures/launch.bmp"); 

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

// Routine to load a program-generated image as a texture. 
void loadProceduralTextures()			
{
   // Activate texture index texture[1]. 
   glBindTexture(GL_TEXTURE_2D, texture[1]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, chessboard);
}

// Create 64 x 64 RGB image of a chessboard.
void createChessboard(void)
{
   int i, j;
   for (i = 0; i < 64; i++) 
      for (j = 0; j < 64; j++) 
         if ( ( ((i/8)%2) && ((j/8)%2) ) || ( !((i/8)%2) && !((j/8)%2) ) )
		 {
            chessboard[i][j][0] = 0x00;
            chessboard[i][j][1] = 0x00;
            chessboard[i][j][2] = 0x00;
		 }
		 else
		 {
            chessboard[i][j][0] = 0xFF;
            chessboard[i][j][1] = 0xFF;
            chessboard[i][j][2] = 0xFF;
		 }
}

// Initialization routine.
void setup(void)
{    
   glClearColor(0.8, 0.8, 0.8, 0.0); 

   // Create texture index array.
   glGenTextures(2, texture); 

   // Load external texture and generate and load procedural texture.
   loadExternalTextures();
   createChessboard();
   loadProceduralTextures();

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
   gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glRotatef(angle, 0.0, 1.0, 0.0); 

   // Activate a texture.
   glBindTexture(GL_TEXTURE_2D, texture[id]);        

   // Map the texture onto a square polygon.
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -10.0, 0.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(10.0, -10.0, 0.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 10.0, 0.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-10.0, 10.0, 0.0);
   glEnd();

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
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
         id++;
		 if (id == 2) id = 0; 
         glutPostRedisplay();
         break;
      case 127:
		 angle = 0.0; 
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_LEFT) 
   {
      angle -= 5.0;
      if (angle < 0.0) angle += 360.0;
   }
   if (key == GLUT_KEY_RIGHT) 
   {
      angle += 5.0;
      if (angle > 360.0) angle -= 360.0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left and right arrow keys to rotate the square." << endl
	    << "Press space to toggle between textures." << endl
        << "Press delete to reset." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("loadTextures.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop();

   return 0; 
}
