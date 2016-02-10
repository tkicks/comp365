/*
Name:		Tyler Kickham
Program:	Interactive Wheaton College campus map
Purpose:	Use C++ and OpenGL to create an interactive map of the Wheaton College campus
Input:		The user will input using the keyboard (will add mouse for menu functionality to click name of a building to be highlighted)
			to: c - highlight the chapel
				d - highlight all dorms
				f - highlight the field house
				l - highlight the library
				s - highlight Mars Science Center
				R - toggle roads on/off
				L - toggle labels on/off
Output:		A computer graphic window will be displayed which will show the Wheaton College
			campus map, drawn with all buildings, paths, roads, and geographical features.
			Labels will be available to be toggled on/off (not sure of default yet).
			See input for list of interactions with map which will change the output.
*/

#include <stdio.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

void toggleLabels();
void toggleRoads();
void drawRoads();

const int wh = 938;			// window height proportions based off of a 5x8 printout
const int ww = 1500;		// window width proportions based off of a 5x8 printout
bool labels = false;
bool roads = true;


void display ()
{
   // set up window
   glClear (GL_COLOR_BUFFER_BIT);

   // draw the roads
   drawRoads();

   // draw
   glFlush ();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key) {
		case 'R': toggleRoads();
				  break;
		case 'L': toggleLabels();
				  break;
		case 'q': exit(1);
	}
}

void init ()
{
   glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

   // background color (green for the grass)
   glClearColor (1.0, 1.0, 1.0, 0.0);

   // flip vertex locations
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   
   // define coordinate system in x, y 0-1
   gluOrtho2D (0.0, ww, 0.0, wh);
}

void toggleLabels()
// toggles the labels on the map
// (doesn't currently have any text so can't test that yet)
{
	if (labels == true)
		labels = false;
	else
		labels = true;
}

void toggleRoads()
// toggles the roads on the map
{
	if (roads == true)
	{
		roads = false;
		drawRoads();
	}
	else
	{
		roads = true;
		drawRoads();
	}
}

void drawRoads()
// draws roads, maps out from West to East
// ww = window width, wh = window height
{
	if (roads == 1)
		// set road color to grey
   		glColor3f(0.5, 0.5, 0.5);
   	else
   		// set road color to green for the grass
   		glColor3f(1.0, 1.0, 1.0);

	// Rt 123
	glRectf((.25*ww), wh, ww, (.98*wh));
	// slant
	glBegin(GL_POLYGON);
		glVertex3f((.15*ww), (.98*wh), 0.0);
		glVertex3f(0.0, (.88*wh), 0.0);
		glVertex3f(0.0, (.86*wh), 0.0);
		glVertex3f((.25*ww), (.98*wh), 0.0);
	glEnd();

	// library square
	glRectf((.0625*ww), wh, (.25*ww), (.98*wh));

	// Rt 140 (library square)
	glRectf((.0625*ww), wh, (.075*ww), (.90*wh));

	// Taunton Ave
	glRectf((.1*ww), 0.0, (.1125*ww), (.93*wh));

	// Howard Street
	glRectf((.25*ww), wh, (.2625*ww), (.4*wh));

	// Howard Street (East-West)
	glRectf((ww));

	// draw roads
	glFlush();
}

int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

	// size of window w/ const global (width, height)
	glutInitWindowSize (ww, wh); 

	// position in top right
	glutInitWindowPosition (50, 0);

	// make window w/ title
	glutCreateWindow ("Campus Map");

	// initialize
	init ();

	// continuously call keyboard function
	glutKeyboardFunc(keyboard);

	// continuously call display function
	glutDisplayFunc(display);

	// keep repeating
	glutMainLoop();
}