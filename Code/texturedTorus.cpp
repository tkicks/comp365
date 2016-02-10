///////////////////////////////////////////////////////////////////////////////////////////
// texturedTorus.cpp
//
// This program, based on torus.cpp, maps a chessboard texture onto a torus. The texture is
// be animated by changing the texture co-ordinates.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////////////////////
  
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
#define R 2.0
#define r 0.5

using namespace std;

// Globals.
static int p = 15; // Number of grid columns.
static int q = 12; // Number of grid rows
static float *vertices = NULL; // Vertex array of the mapped sample on the torus.
static float *textureCoordinates = NULL; // Texture co-ordinates array of the mapped sample on the torus.
static unsigned int texture[1]; // Array of texture indices.
static unsigned char chessboard[64][64][3]; // Storage for chessboard image.
static float shift = 0.0; // Texture shift.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.

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

// Routine to load a chessboard image as a texture. 
void loadProceduralTextures()			
{
   // Bind chessboard image to texture index[0].
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, chessboard);
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
            chessboard[i][j][1] = 0x00;
            chessboard[i][j][2] = 0x00;
		 }
}

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the torus.
float f(int i, int j)
{
   return ( ( R + r * cos( (-1 + 2*(float)j/q) * PI ) ) * cos( (-1 + 2*(float)i/p) * PI ) );
}

float g(int i, int j)
{
   return ( ( R + r * cos( (-1 + 2*(float)j/q) * PI ) ) * sin( (-1 + 2*(float)i/p) * PI ) ); 
}

float h(int i, int j)
{
   return ( r * sin( (-1 + 2*(float)j/q) * PI ) );
}

// Routine to fill the vertex array with co-ordinates of the mapped sample points.
void fillVertexArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= q; j++)
      for (i = 0; i <= p; i++)
      {
         vertices[k++] = f(i,j);
         vertices[k++] = g(i,j);
         vertices[k++] = h(i,j);
      }
}

// Routine to fill the texture co-ordinates array with the texture co-ordinate values at the mapped sample points.
void fillTextureCoordArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= q; j++)
      for (i = 0; i <= p; i++)
      {
		 // If shift = 0, the point on the torus with parameters (u, v) is mapped to the
		 // point (u, v) of texture space. The animation routine periodically increments 
		 // shift to cause the texture to slide around the torus.
         textureCoordinates[k++] = (float)i/p;
         textureCoordinates[k++] = (float)j/q + shift; 
      }
}

// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      shift += 0.01;
	  if (shift > 1.0) shift -= 1.0;
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

// Initialization routine.
void setup(void)
{    
   glEnable(GL_DEPTH_TEST); // Enable depth testing.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);

   glClearColor(1.0, 1.0, 1.0, 0.0); 

   // Create texture index array.
   glGenTextures(1, texture); 

   // Generate and load chessboard texture.
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
   int  i, j;
   vertices = new float[3*(p+1)*(q+1)]; // Dynamic array allocation with new value of p and q. 
   textureCoordinates = new float[2*(p+1)*(q+1)];

   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates); 
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Fill the vertex and texture co-ordinates arrays.
   fillVertexArray();
   fillTextureCoordArray();

   // Map the chessboard texture onto the torus.
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   for(j = 0; j < q; j++)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i <= p; i++)
      {
         glArrayElement( (j+1)*(p+1) + i );
         glArrayElement( j*(p+1) + i );
	  }
      glEnd();
   }

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
         if (isAnimate) isAnimate = 0;
		 else isAnimate = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) animationPeriod += 5;
   if( key == GLUT_KEY_UP) if (animationPeriod > 5) animationPeriod -= 5;
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("texturedTorus.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop();

   return 0; 
}
