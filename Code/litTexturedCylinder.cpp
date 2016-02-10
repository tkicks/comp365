/////////////////////////////////////////////////////////////////////////////////////////
// litTexturedCylinder.cpp
//
// This programs builds on litCylinder.cpp by texturing the cylinder. 
//
// NOTE: The Textures folder must be in the same one as this program.
//
// Interaction:
// Press x, X, y, Y, z, Z to turn the cylinder.
// 
// Sumanta Guha.
/////////////////////////////////////////////////////////////////////////////////////////
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
static int p = 30; // Number of grid columns.
static int q = 10; // Number of grid rows
static float R = 0.75; // Radius of can.
static float *vertices = NULL; // Vertex array of the mapped sample on the cylinder.
static float *normals = NULL; // Normal array of the mapped sample on the cylinder.
static float *textureCoordinates = NULL; // Texture co-ordinates array of the mapped sample on the cylinder.
static float Xangle = 315.0, Yangle = 30.0, Zangle = 0.0; // Angles to rotate cylinder.
static unsigned int texture[2]; // Array of texture indices.

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
   BitMapFile *image[2]; 
   
   // Load the textures.
   image[0] = getBMPData("Textures/canLabel.bmp"); 
   image[1] = getBMPData("Textures/canTop.bmp");   

   // Bind can label image to texture index[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind can top image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);	
}

// Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the cylinder.
float f(int i, int j)
{
   return ( R * cos( (-1 + 2*(float)i/p) * PI ) );
}

float g(int i, int j)
{
   return ( R * sin( (-1 + 2*(float)i/p) * PI ) );
}

float h(int i, int j)
{
   return ( -1 + 2*(float)j/q );
}

// Fuctions to map the grid vertex (u_i,v_j) to the normal (fn(u_i,v_j), gn(u_i,v_j), hn(u_i,v_j)) to the cylinder 
// at the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)).
float fn(int i, int j)
{
   return ( cos( (-1 + 2*(float)i/p) * PI ) );
}

float gn(int i, int j)
{
   return ( sin( (-1 + 2*(float)i/p) * PI ) );
}

float hn(int i, int j)
{
   return ( 0 );
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

// Routine to fill the normal array with normal vectors at the mapped sample points.
void fillNormalArray(void)
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= q; j++)
      for (i = 0; i <= p; i++)
      {
         normals[k++] = fn(i,j);
         normals[k++] = gn(i,j);
         normals[k++] = hn(i,j);
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
         textureCoordinates[k++] = (float)i/p;
         textureCoordinates[k++] = (float)j/q;
      }
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST);  

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);
   
   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightPos[] = { 0.0, 1.5, 3.0, 0.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
   
   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
   glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); // Enable separate specular light calculation.

   // Material property vectors.
   float matAmbAndDif[] = {1.0, 1.0, 1.0, 1.0};
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);

   vertices = new float[3*(p+1)*(q+1)];  
   normals = new float[3*(p+1)*(q+1)]; 
   textureCoordinates = new float[2*(p+1)*(q+1)]; 

   fillVertexArray();
   fillNormalArray();
   fillTextureCoordArray();

   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glNormalPointer(GL_FLOAT, 0, normals);
   glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);  

   // Create texture index array and load external textures.
   glGenTextures(2, texture);
   loadExternalTextures();

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D); 

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
}

// Drawing routine.
void drawScene(void)
{
   int  i, j;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt (0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   // Map the label texture onto the cylinder.
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

   // Map the can top texture onto a disc at the top of the cylinder.
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glBegin(GL_POLYGON);
   glTexCoord2f(0.0, 0.0);
   for(i = 0; i <= p; i++)
   {
	  glNormal3f(0.0, 0.0, 1.0);
	  glTexCoord2f( 0.5 + 0.5*cos( (-1 + 2*(float)i/p) * PI ), 0.5 + 0.5*sin( (-1 + 2*(float)i/p) * PI ) );
      glVertex3f( R * cos( (-1 + 2*(float)i/p) * PI ), R * sin( (-1 + 2*(float)i/p) * PI ), 1.0 );   
   }
   glEnd();

   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case 'x':
         Xangle += 5.0;
		 if (Xangle > 360.0) Xangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'X':
         Xangle -= 5.0;
		 if (Xangle < 0.0) Xangle += 360.0;
         glutPostRedisplay();
         break;
      case 'y':
         Yangle += 5.0;
		 if (Yangle > 360.0) Yangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Y':
         Yangle -= 5.0;
		 if (Yangle < 0.0) Yangle += 360.0;
         glutPostRedisplay();
         break;
      case 'z':
         Zangle += 5.0;
		 if (Zangle > 360.0) Zangle -= 360.0;
         glutPostRedisplay();
         break;
      case 'Z':
         Zangle -= 5.0;
		 if (Zangle < 0.0) Zangle += 360.0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press x, X, y, Y, z, Z to turn the cylinder." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("litTexturedCylinder.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutMainLoop();

   return 0;
}

