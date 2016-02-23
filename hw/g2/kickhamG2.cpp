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
don't rotate 45 just 90
*/

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
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
		int getLength();							// get length of segments
		int getSize();								// get size of vector of points
		void rotateMath(float x1, float y1, float x2, float y2);	// do math for rotation
		vector<float> getLastCoords();				// get the last coordinates in the fractal
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
	int size = fractalPoints.size();				// size of vector of points

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.0);
	// if it's the first point, draw another a segment length to the right
	if (size == 1)
	{
		// fractal.setNext((fractalPoints[0][0] + segLength), fractalPoints[0][1]);
		for (int i = 0; i < size; i++)		// for each new coord in fractal
		{
			// cout << fractalPoints.size() << endl;
			tempVector.push_back(fractalPoints[i][0]+segLength);		// put next x in intermediary vector
			tempVector.push_back(fractalPoints[i][1]);					// put next y in intermediary vector
			fractalPoints.push_back(tempVector);						// put new fractal into fractal vector
			tempVector.clear();
		}
	}
	else													// otherwise
	{
		for (int i = size-1; i > 0; i--)		// for each point
		{
			float x1 = fractalPoints[i][0];		// x1 (current) value for rotation
			float y1 = fractalPoints[i][1];		// y1 (current) value for rotation
			float x2 = fractalPoints[i-1][0];	// previous x value for rotation
			float y2 = fractalPoints[i-1][1];	// previous y value for rotation
			fractal.rotateMath(x1, y1, x2, y2);	// calculate new x, y value
		}
	}


	if (size > 10)
		glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < fractalPoints.size(); i++)
		{
			cout << fractalPoints[i][0] << " " << fractalPoints[i][1] << endl;
			glVertex3f(fractalPoints[i][0], fractalPoints[i][1], 0.0);
		}
		cout << endl;
	glEnd();

	glFlush();			// draw fractal
}

void dragonFractal::setNext(float x, float y)
// INPUT: float x and y coords to start at 	OUTPUT: none
// set the starting x and y coords
{
	// cout << x << " " << y << endl;
	this->tempVector.push_back(x);				// next spot in vector gets x coord
	this->tempVector.push_back(y);				// then y coord of next point
	this->fractalPoints.push_back(tempVector);	// push new coords to 2d vector of points
	this->tempVector.clear();					// empty out intermediary vector so no repeats
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

int dragonFractal::getSize()
// INPUT: none	OUTPUT: number of points in fractal
// returns the size of vector for mouse function since it isn't a class
// function and therefore vector not available
{
	return fractalPoints.size();
}

int dragonFractal::getLength()
// INPUT: none	OUTPUT: number of points in vector
// return number of points in fractal
{
	return fractalPoints.size();
}

void dragonFractal::rotateMath(float x1, float y1, float x2, float y2)
// INPUT: x1y1 = coords for RxRy (anchor of rotation)
//		  x2y2 = coords of other end of rotation
// OUTPUT: none
// FIX SO THAT IT MOVES FROM LAST POINT NOT ORIGINAL POINT BASED OFF OF
{
	float newX, newY;				// new x and y values
	
	// if last direction was right go up
	if (x1 > x2)
	{
		newX = fractalPoints.back()[0];
		newY = fractalPoints.back()[1] + this->segLength;
	}

	// if last direction was down go right
	else if (y1 < y2)
	{
		newX = fractalPoints.back()[0] + this->segLength;
		newY = fractalPoints.back()[1];
	}

	// if last direction was left go down
	else if (x1 < x2)
	{
		newX = fractalPoints.back()[0];
		newY = fractalPoints.back()[1] - this->segLength;
	}

	// if last direction was up go left
	else
	{
		newX = fractalPoints.back()[0] - this->segLength;
		newY = fractalPoints.back()[1];
	}

	// set new coords
	fractal.setNext(newX, newY);
}


vector<float> dragonFractal::getLastCoords()
// INPUT: none	OUTPUT: vector containing last x, y coords in fractal
// return the last x, y coords in the fractal
{
	return fractalPoints.back();
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
	// get last point
	int size = fractal.getSize();							// size of vector of points (not class function so needs getter)
	int length = fractal.getLength();						// get length of segment
	vector<float> lastCoords = fractal.getLastCoords();		// get last coordinates

	// if the left mouse was pressed and it wasn't in the menu draw fractal
	if (state == GLUT_DOWN && !fractal.getInMenu())
	{
		// fractal.setNext((lastCoords[0]+length), (lastCoords[1]+length));
		fractal.drawFractal();
	}

	lastCoords.clear();		
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
	// set default segment length to 20
	fractal.setLength(0.2);
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