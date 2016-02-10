///////////////////////////////////////////////////////////////////////////////////////////////
// ballAndTorusShadowed.cpp
//
// This program, based on ballAndTorus.cpp, adds a directional light source, a checkered floor,
// and projects the shadow of the ball and torus on the floor by an application of a 
// degenerate scaling transformation.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press the up/down arrow keys to speed up/slow down animation.
//
// Sumanta Guha
///////////////////////////////////////////////////////////////////////////////////////////////

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

// Draw ball flying around torus, both black if shadow is true, colored otherwise.
void drawFlyingBallAndTorus(int shadow)
{
   glPushMatrix();

   glTranslatef(0.0, 10.0, 0.0);
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
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING); 

   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightPos[] = {0.0, 1.0, 0.0, 0.0 }; // Overhead directional light source (e.g., sun).
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
 
   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

	// Material property vectors.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Enable color material mode.
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

// Drawing routine.
void drawScene(void)
{
   int i = 0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();
   gluLookAt(0.0, 5.0, 30.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
 
   // Draw checkered floor.
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

   glDisable(GL_LIGHTING); // Turn off lighting as shadows are not lit.
   glDisable(GL_DEPTH_TEST); // Turn off depth testing so that shadows are drawn over the
                             // floor without comparing distance from the viewer.
   
   // Draw shadows of the real ball and torus by projecting them onto the xz-plane using
   // a degenerate scaling transformation that collapses y-values.
   glPushMatrix();
   glScalef(1.0, 0.0, 1.0);
   drawFlyingBallAndTorus(1);
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
   glutCreateWindow("ballAndTorusShadowed.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop();

   return 0; 
}
