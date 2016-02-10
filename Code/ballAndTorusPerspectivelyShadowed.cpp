////////////////////////////////////////////////////////////////////////////////////////////
// ballAndTorusPerspectivelyShadowed.cpp
//
// This program, based on ballAndTorusShadowed.cpp, adds a second positional light source 
// located at the origin, and computes the so-called perspective shadow cast by this source 
// on a back wall by an application of a projective transformation.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
//
// Sumanta Guha
////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals. 
static float latAngle = 0.0; // Latitudinal angle.
static float longAngle = 0.0; // Longitudinal angle.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.
static float matrixData[16]; // Array to store a transformation matrix entries.

// Draw ball flying around torus, both black if shadow is true, colored otherwise.
void drawFlyingBallAndTorus(int shadow)
{
   glPushMatrix();

   glTranslatef(0.0, 10.0, -15.0);
   glRotatef(90.0, 1.0, 0.0, 0.0);

   // Fixed torus.
   if ( shadow ) glColor3f(0.0, 0.0, 0.0);
   else glColor3f(0.0, 1.0, 0.0);
   glutSolidTorus(2.0, 12.0, 80, 80);

   // Begin revolving ball.
   glRotatef(longAngle, 0.0, 0.0, 1.0);
   
   glTranslatef(12.0, 0.0, 0.0);
   glRotatef(latAngle, 0.0, 1.0, 0.0);
   glTranslatef(-12.0, 0.0, 0.0);
   
   glTranslatef(20.0, 0.0, 0.0);

   if ( shadow ) glColor3f(0.0, 0.0, 0.0);
   else glColor3f(0.0, 0.0, 1.0);
   glutSolidSphere(2.0, 20, 20);
   // End revolving ball.

   glPopMatrix();
}

// Routine to draw a checkered plane.
void drawCheckeredPlane(void)
{
   int i = 0;
   
   glShadeModel(GL_FLAT); // Flat shading to get the checkered pattern.
   for(float z = 100.0; z > -100.0; z -= 5.0)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for(float x = -100.0; x < 100.0; x += 5.0)
	  {
         if (i % 2) glColor3f(0.0, 0.5, 0.5);  
         else glColor3f(1.0, 1.0, 1.0); 
         glNormal3f(0.0, 1.0, 0.0);
		 glVertex3f(x, 0.0, z - 5.0);
	     glVertex3f(x, 0.0, z);
		 i++;
	  }
      glEnd();
	  i++;
   }
   glShadeModel(GL_SMOOTH); // Restore smooth shading.
}
 
// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
      latAngle += 5.0;
	  if (latAngle > 360.0) latAngle -= 360.0;
      longAngle += 1.0;
	  if (longAngle > 360.0) longAngle -= 360.0;
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
}

// Initialization routine.
void setup(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); 

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING); 

   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec0[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightDifAndSpec1[] = { 0.1, 0.1, 0.1, 1.0 };
   float lightPos0[] = {0.0, 1.0, 0.0, 0.0 }; // Overhead directional light source (e.g., sun).
   float lightPos1[] = {0.0, 0.0, 0.0, 1.0 }; // Positional light source at the origin.
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light0 properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

   // Light1 properties.
   glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
   glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);
   glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

   glEnable(GL_LIGHT0); // Enable directional light source.
   glEnable(GL_LIGHT1); // Enable positional light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

	// Material property vectors.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 5.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Enable color material mode.
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   // Enter the projection matrix stack to use OpenGL in order to create a projection
   // matrix for a frustum with the near plane at z=-35 and store it in matrixData. 
   // This matrix will be used for shadow calculation for a light source at the origin.
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glFrustum(-1.0, 1.0, -1.0, 1.0, 35.0, 100.0);
   glGetFloatv(GL_PROJECTION_MATRIX, matrixData);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW); // Exit the projection matrix stack.
}

// Drawing routine.
void drawScene(void)
{
   int i = 0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 5.0, 30.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);

   // Draw floor.
   drawCheckeredPlane();

   // Draw back wall.
   glPushMatrix();
   glTranslatef(0.0, 0.0, -35.);
   glRotatef(90.0, 1.0, 0.0, 0.0);
   drawCheckeredPlane();
   glPopMatrix();

   glDisable(GL_LIGHTING); // Turn off lighting as shadows are not lit.
   glDisable(GL_DEPTH_TEST); // Turn off depth testing so that shadows are drawn over the
                             // floor and wall without comparing distance from the viewer.

   // Draw shadows of the real ball and torus, cast by the overhead light source, by projecting 
   // them onto the xz-plane using a degenerate scaling transformation that collapses the y-values.
   glPushMatrix();
   glScalef(1.0, 0.0, 1.0);
   drawFlyingBallAndTorus(1);
   glPopMatrix();

   // Draw a shadow of the real ball and torus cast on the back wall (at z=-35) by the light 
   // source at the origin, by 
   // (a) transforming the ball and torus with a projective transformation that maps a frustum 
   //     with base at at z=-35 to an axes-parallel box, then
   // (b) collapsing z-values with a degenerate scaling transformation, and, finally,
   // (c) translating the shadow so that it lies on the back wall.
   glPushMatrix();
   glTranslatef(0.0, 0.0, -35.0); // Translate the shadow to the back wall.
   glScalef(1.0, 1.0, 0.0); // Collapse z-values to make a 2D shadow.
   glMultMatrixf(matrixData); // Projective transformation to the canonical box.
   drawFlyingBallAndTorus(1); // Draw black ball and torus.
   glPopMatrix();

   glEnable(GL_DEPTH_TEST); // Restore depth testing.
   glEnable(GL_LIGHTING); // Restore lighting.

   // Draw real ball and torus.
   drawFlyingBallAndTorus(0);

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
	  case ' ': 
         if (isAnimate) isAnimate = 0;
		 else isAnimate = 1;
         glutPostRedisplay();
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
   glutCreateWindow("ballAndTorusPerspectivelyShadowed.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop();

   return 0; 
}
