/*
Name:		Tyler Kickham
Program:	Dragon Fractal
Purpose:	Use C++ and OpenGL to create an interactive dragon fractal
Input:		The user will input using the mouse
			to: right click a menu w/ options
				to: clear window (doesn't delete image)
					redisplay the fractal (like after window clear)
					create new default (predetermined location and length)
					create new user (user inputs location and length)
					choose segment length for current level of fractal (can change in middle)
					quit
				left click iterate to next level of fractal
Output:		A computer graphic window will be displayed which will initially
			be empty.  With each keypress (as explained in input) the
			fractal will expand or shrink (and maybe change colors).

set segment length by keyboard input of number of pixels
(1-9 unless figure out how else)
*/

#include <GL/glut.h>
#include <iostream>
#include <vector>
using namespace std;

class dragonFractal
{
	public:
		dragonFractal() {};
		~dragonFractal() {};
		void drawFractal();							// function to draw fractal
		bool getInMenu();							// return if left click happened in menu
		void setInMenu(bool);						// set inMenu value to true or false
		void setNext(float x, float y);				// set start x and y coords
		void setLength(float length);				// set segment length
	private:
		int levels;									// number of levels in the fractal
		int pointNum;								// number of points it's on for vector iterations
		float location;								// initial location
		float segLength;							// length of segments
		bool inMenu;								// was left click in menu?
		vector<vector <float> > fractalPoints;		// vector of points in fractal [i][0] = x [i][1] = y
		vector<float> tempVector;					// intermediary vector for 2d vector
};

const int iterationLimit = 3;		// maximum number of iterations allowed
const float ww = 500.0;				// set window's width
const float wh = 500.0;				// set window's height
dragonFractal fractal;				// create an object to use

void dragonFractal::drawFractal()
// INPUT: none?	OUTPUT: none
// draw next fractal level
{
	// cout << "Draw fractal\n";
	cout << fractalPoints.size() << endl;
	// if it's the first point, draw another a segment length to the right
	if (fractalPoints.size() == 1)
	{
		fractal.setNext(fractalPoints[0][0], (fractalPoints[0][1] + segLength));
	}
	else
	{
		// fractal.setNext(fractalPoints[pointNum][0])
	}
}

void dragonFractal::setNext(float x, float y)
// INPUT: float x and y coords to start at 	OUTPUT: none
// set the starting x and y coords
{
	this->tempVector.push_back(x);				// next spot in vector gets x coord
	this->tempVector.push_back(y);				// then y coord of next point
	this->fractalPoints.push_back(tempVector);	// push new coords to 2d vector of points
	this->tempVector.clear();					// empty out intermediary vector so no repeats

	// test storage
	// for (int i = 0; i < fractalPoints.size(); i++)
	// 	cout << fractalPoints[i][0] << " " << fractalPoints[i][1] << endl;				// see what's stored
}

void dragonFractal::setLength(float length)
// INPUT; float segment length (1-9)	OUTPUT: none
// sets the segment length (can be modified on the fly)
{
	this->segLength = length;
}

void dragonFractal::setInMenu(bool value)
// INPUT: true or false bool value	OUTPUT: none
// set the value of in menu to true or false
{
	this->inMenu = value;
	cout << "in menu? " << this->inMenu << endl;
}

bool dragonFractal::getInMenu()
// INPUT: none	OUTPUT: true if clicked while in menu false if clicked out of menu
// checks what to return for output
{
	return this->inMenu;
}


void display ()
// INPUT: none	OUTPUT: none
// create the display
{
	dragonFractal fractal;			// instantiate a class object
	// set up window
	glClear (GL_COLOR_BUFFER_BIT);

	// draw
	glFlush ();
}

void mouse (int button, int state, int x, int y)
// INPUT: which button pressed on mouse, state of object, x/y coords of click
// OUTPUT: none
// increase fractal level
{
	// if the left mouse was pressed and it wasn't in the menu draw fractal
	if (state == GLUT_DOWN && !fractal.getInMenu())
		fractal.drawFractal();
}

void menu (int menuVal)
// INPUT: value of menu option clicked	OUTPUT: none
// call function to do based on menu option chosen
{
	fractal.setInMenu(true);		// say in menu
	// holder
	switch (menuVal)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4: cout << "Menu option " << menuVal << " chosen\n";
				fractal.setInMenu(false);
				break;
		case 5: exit(1);
	}
}

void init ()
// INPUT: none	OUTPUT: none
// initiate windows and set up coordinate locations
{
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// background color (green for the grass)
	glClearColor (1.0, 1.0, 1.0, 0.0);

	// flip vertex locations
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	// define coordinate system in x, y 0-1
	gluOrtho2D (0.0, ww, 0.0, wh);

	// set default starting location to center of screen (250, 250)
	fractal.setNext(250.0, 250.0);
	// set default segment length to 5
	fractal.setLength(5.0);
}

void initMenu ()
// INPUT: fractal object	OUTPUT: none
// initialize the menu on a right mouse click
{
	glutCreateMenu(menu);			// call the menu function
	glutAddMenuEntry("Create New (default)", 0);
	glutAddMenuEntry("Create New (custom)", 1);
	glutAddMenuEntry("Segment Length", 2);
	glutAddMenuEntry("Clear", 3);
	glutAddMenuEntry("Redisplay", 4);
	glutAddMenuEntry("Quit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main (int argc, char** argv)
// INPUT: none	OUTPUT: none
// main function, calls other functions
{
	// console instructions
	cout << "Right click for menu\nLeft click to iterate fractal\n";
	
	// initiate window/viewport
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

	// size of window w/ const global (width, height)
	glutInitWindowSize (ww, wh); 

	// position in top left
	glutInitWindowPosition (50, 0);

	// make window w/ title
	glutCreateWindow ("Dragon Fractal");

	// initialize
	init ();

	// initialize menu
	initMenu();

	// continuously call display function
	glutDisplayFunc(display);

	// continuously call mouse function
	glutMouseFunc(mouse);

	// keep repeating
	glutMainLoop();
}