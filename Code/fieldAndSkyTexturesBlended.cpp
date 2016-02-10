////////////////////////////////////////////////////////////////////////////////////////
// fieldAndSkyTexturesBlended.cpp
//
// This program, based on fieldAndSkyLit.cpp, blends a day sky texture onto a night sky 
// texture as the angle of the directional light source (sun) changes.
//
// NOTE: The Textures folder must be in the same one as this program.
//
// Interaction:
// Press the arrow keys to change the direction of the sun.
//
// Sumanta Guha
////////////////////////////////////////////////////////////////////////////////////////
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
static unsigned int texture[3]; // Array of texture indices.
static float theta = 0.0; // Angle of the sun with the ground.

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
   BitMapFile  *image[3]; 
   
   // Load the textures.
   image[0] = getBMPData("Textures/grass.bmp");
   image[1] = getBMPData("Textures/sky.bmp");   
   image[2] = getBMPData("Textures/nightsky.bmp");  

   // Bind grass image to texture index[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

   // Bind sky image to texture index[1]
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);
   
   // Bind night sky image to texture index[2]
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2]->sizeX, image[2]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[2]->data);	
}

// Initialization routine.
void setup(void)
{    
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);   

   // Light property vectors that are constant.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

   // Material property vectors that are constant.
   float matAmbAndDif[] = {1.0, 1.0, 1.0, 1.0};
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Create texture index array and load external textures.
   glGenTextures(3, texture);
   loadExternalTextures();

   // Turn on OpenGL texturing.
   glEnable(GL_TEXTURE_2D);   

   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

   glEnable(GL_BLEND); // Enable blending.

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

// Drawing routine.
void drawScene(void)
{
   float alpha; // Blending parameter.

   // Light property vectors that change during runtime.
   float lightPos[] = { cos( (PI/180.0)*theta ), sin( (PI/180.0)*theta ), 0.0, 0.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 10.0, 15.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
   
   // Blend the grass texture onto a rectangle lying along the xz-plane.
   glBlendFunc(GL_ONE, GL_ZERO); // Specify blending parameters to overwrite background.
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_POLYGON);
      glNormal3f(0.0, 1.0, 0.0);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, 100.0);
      glTexCoord2f(8.0, 0.0); glVertex3f(100.0, 0.0, 100.0);
      glTexCoord2f(8.0, 8.0); glVertex3f(100.0, 0.0, -100.0);
      glTexCoord2f(0.0, 8.0); glVertex3f(-100.0, 0.0, -100.0);
   glEnd();

   glDisable(GL_LIGHTING); // The sky textures don't need to be lit.

   // Blend the night sky texture onto a rectangle parallel to the xy-plane.
   glBlendFunc(GL_ONE, GL_ZERO); // Specify blending parameters to overwrite background.
   glBindTexture(GL_TEXTURE_2D, texture[2]);        
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -70.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, -70.0);
   glEnd();
   
   // The blending parameter varies with the angle of the sun.
   if (theta <= 90.0) alpha = theta/90.0; 
   else alpha = (180.0 - theta)/90.0;    
   glColor4f(1.0, 1.0, 1.0, alpha); 

   // Blend the sky texture onto the same rectangle as the night sky texture -
   // disabling the depth test allows this as both have the same z-values
   glDisable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters to mix skies.
   glBindTexture(GL_TEXTURE_2D, texture[1]);        
   glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 0.0, -70.0);
      glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 120.0, -70.0);
      glTexCoord2f(0.0, 1.0); glVertex3f(-100.0, 120.0, -70.0);
   glEnd();
   glEnable(GL_DEPTH_TEST);

   // Draw a white line in the direction of the sun with lighting still disabled.
   glColor3f(1.0, 1.0, 1.0);
   glLoadIdentity();
   glBegin(GL_LINES);
      glVertex3f(0.0, 0.0, -6.0);
	  glVertex3f( cos( (PI/180.0)*theta ), sin( (PI/180.0)*theta ), -6.0 );
   glEnd();

   glEnable(GL_LIGHTING); // Restore lighting.

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
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_RIGHT) 
   {
      if (theta > 0.0) theta -= 1.0;
   }
   if (key == GLUT_KEY_LEFT) 
   {
      if (theta < 180.0) theta += 1.0;
   }
   if (key == GLUT_KEY_UP) 
   {
      if (theta < 90.0) theta += 1.0;
      if (theta > 90.0) theta -= 1.0;
   }
   if (key == GLUT_KEY_DOWN) 
   {
      if ( (theta <= 90.0) && (theta > 0.0) ) theta -= 1.0;
      if ( (theta > 90.0) && (theta < 180.0) )theta += 1.0;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the arrow keys to change the direction of the sun." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("fieldAndSkyTexturesBlended.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop();

   return 0; 
}
