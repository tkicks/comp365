/*
Name:		Tyler Kickham
Program:	Dragon Fractal (kickhamG2.cpp)
Purpose:	Use C++ and OpenGL to create an interactive fractal with menu
			options to change the way it is displayed, sometimes on the fly
Input:		The user will input using the mouse
			to: right click creates a menu w/ options
				to: create new default (predetermined start location and
									segment length)
					create new user (user inputs start location w/ second
									click on window and segment length)
					toggle random colors (on/off)
					choose segment length for current level of fractal
									(will be applied to future iterations)
					type of segments: line strip (default, all lines
									connected, normal dragon fractal)
					type of segments: lines (all segments are disconnected
									only displays horizontal lines, creates
									shutter effect)
					clear window (doesn't delete image)
					redisplay the fractal (use after window clear)
					quit
				left click iterate to next level of fractal (up to 20)
Output:		A computer graphic window will be displayed which will initially
			be empty.  With each left click (up to 20) a dragon fractal will
			be expanded.  A right click will produce a menu with options
			listed in Input above which may change the appearance of the
			fractal.
*/

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <time.h>
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
		void resetLevels();							// reset number of fractal iterations to 0
		void setStart(bool setTo);					// set random starting point
		bool getStart();							// return if starting location is random
		void rotateMath(float x1, float y1, float x2, float y2);	// do math for rotation
		void figureNext();							// figure out next point
		void ToggleRandomColors();					// user toggles whether or not to use random colors
		float colorVal();							// return a random color float to make
		void setLineType(char lineType);			// set which type of line to use
		int getLineType();							// return what kind of line to draw
		vector<float> getLastCoords();				// get the last coordinates in the fractal
	private:
		int levels;									// number of levels in the fractal
		int pointNum;								// number of points it's on for vector iterations
		int lineStyle;								// holds value of enum linestyle to use
		float location;								// initial location
		float segLength;							// length of segments
		bool inMenu;								// was left click in menu?
		bool randomStart;							// is starting point random
		bool NewRandomColor;						// decide if it's time for a new random color
		bool randomColors;							// user choice of whether or not to use random colors
		vector<vector <float> > fractalPoints;		// vector of points in fractal [i][0] = x [i][1] = y
		vector<float> tempVector;					// intermediary vector for 2d vector
};

const int iterationLimit = 20;		// maximum number of iterations allowed
const float ww = 1000.0;			// set window's width
const float wh = 800.0;				// set window's height
dragonFractal fractal;				// create an object to use


void dragonFractal::resetLevels()
// INPUT: none	OUTPUT: none
// reset the number of levels done to zero
{
	fractalPoints.clear();
	this->levels = 0;
}


vector<float> dragonFractal::getLastCoords()
// INPUT: none	OUTPUT: vector containing last x, y coords in fractal
// return the last x, y coords in the fractal
{
	return fractalPoints.back();
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
}

void dragonFractal::setStart(bool setTo)
// INPUT: bool value to set random start to 	OUTPUT: none
// set whether or not to start fractal from point clicked on
{
	this->randomStart = setTo;
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

bool dragonFractal::getStart()
// INPUT: none	OUTPUT: whether or not setting new starting location
// return whether to start drawing fractal where clicked
{
	return this->randomStart;
}

void dragonFractal::ToggleRandomColors()
// INPUT: none	OUTPUT: none
// toggles whether or not to use random colors
{
	if (this->randomColors)
		this->randomColors = false;
	else
		this->randomColors = true;
}

void dragonFractal::setLineType(char lineType)
// INPUT: char deciding what type of line to draw - s = LINE_STRIP l = LINES
// OUTPUT: none
// set line type
{
	if (lineType == 's')
		this->lineStyle = GL_LINE_STRIP;
	else
		this->lineStyle = GL_LINES;
}

int dragonFractal::getLineType()
// INPUT: none	OUTPUT: int depicting enum value for line type
// returns what kind of lines to use when drawing
{
	return this->lineStyle;
}

void dragonFractal::figureNext()
// INPUT: none	OUTPUT: none
// figure out next point
{
	if (this->levels <= iterationLimit)
	{
		int size = fractalPoints.size();				// size of vector of points

		// if it's the first point, draw another a segment length to the right
		if (size == 1)
		{
			for (int i = 0; i < size; i++)		// for each new coord in fractal
			{
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

		this->pointNum = fractalPoints.size();
		this->levels += 1;
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
}

void dragonFractal::drawFractal()
// INPUT: none?	OUTPUT: none
// draw next fractal level
{
	int linType = fractal.getLineType();
	if (randomColors)
	{
		float r = fractal.colorVal()/10;
		float g = fractal.colorVal()/10;
		float b = fractal.colorVal()/10;
		glColor3f(r, g, b);
	}
	else
		glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.0);
	glBegin(linType);
		for (int i = 0; i < fractalPoints.size(); i++)
		{
			if (i > 25000 && i < 85000 && this->NewRandomColor)
			{
				if (randomColors)
				{
					float r = fractal.colorVal()/10;
					float g = fractal.colorVal()/10;
					float b = fractal.colorVal()/10;
					glColor3f(r, g, b);
					this->NewRandomColor = false;
				}
				else
					glColor3f(0.0, 1.0, 0.0);
			}
			else if (i > 85000 && i < 255000 && this->NewRandomColor)
			{
				if (randomColors)
				{
					float r = fractal.colorVal()/10;
					float g = fractal.colorVal()/10;
					float b = fractal.colorVal()/10;
					glColor3f(r, g, b);
					this->NewRandomColor = false;
				}
				else
					glColor3f(0.0, 0.0, 1.0);
			}
			else if (i > 255000 && i < 450000 && this->NewRandomColor)
			{
				if (randomColors)
				{
					float r = fractal.colorVal()/10;
					float g = fractal.colorVal()/10;
					float b = fractal.colorVal()/10;
					glColor3f(r, g, b);
					this->NewRandomColor = false;
				}
				else
					glColor3f(1.0, 1.0, 0.0);
			}
			else if (i > 450000 && this->NewRandomColor)
			{
				if (randomColors)
				{
					float r = fractal.colorVal()/10;
					float g = fractal.colorVal()/10;
					float b = fractal.colorVal()/10;
					glColor3f(r, g, b);
					this->NewRandomColor = false;
				}
				else
					glColor3f(1.0, 1.0, 1.0);
			}
			glVertex3f(fractalPoints[i][0], fractalPoints[i][1], 0.0);
			if (i == 25000 || i == 85000 || i == 255000 || i == 450000)
				this->NewRandomColor = true;
		}
	glEnd();

	glFlush();			// draw fractal
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

float dragonFractal::colorVal()
// INPUT: none	OUTPUT: float RGB value to set next color to
// generate and return a random float between 0.0 and 1.0
{
	return (rand()%9);
}


void display ()
// INPUT: none	OUTPUT: none
// create the display
{
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
	if (button == GLUT_RIGHT_BUTTON)
		return;
	// if the left mouse was pressed and it wasn't in the menu draw fractal
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !fractal.getInMenu())
	{
		if (fractal.getSize() <= 0)
		{
			if (!fractal.getStart())
				fractal.setNext((.75*ww), (.35*wh));
			else if (fractal.getStart())
			{
				fractal.setNext(x, (wh-y));
			}
			fractal.setStart(false);
		}
		fractal.figureNext();

		fractal.drawFractal();
	}
}

void menu (int menuVal)
// INPUT: value of menu option clicked	OUTPUT: none
// call function to do based on menu option chosen
{
	fractal.setInMenu(true);		// say in menu
	float segLen;
	switch (menuVal)
	{
		case 0: fractal.resetLevels();
				fractal.setLength(0.7);
				fractal.setNext((.75*ww), (.35*wh));
				fractal.figureNext();
				fractal.setInMenu(false);
				break;
		case 1: fractal.resetLevels();
				fractal.setInMenu(false);
				cout << "Enter segment length: ";
				cin >> segLen;
				fractal.setLength(segLen);
				fractal.setStart(true);
				break;
		case 2:	cout << "Enter segment length: ";
				cin >> segLen;
				fractal.setLength(segLen);
				fractal.setInMenu(false);
				break;
		case 3: glClearColor(0.21, 0.27, 0.31, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);
				fractal.setInMenu(false);
				break;
		case 4: fractal.drawFractal();
				fractal.setInMenu(false);
				break;
		case 5: exit(1);
		case 6:	fractal.ToggleRandomColors();
				fractal.setInMenu(false);
				break;
		case 7: fractal.setLineType('s');
				fractal.setInMenu(false);
				break;
		case 8: fractal.setLineType('l');
				fractal.setInMenu(false);
				break;
	}
}

void init ()
// INPUT: none	OUTPUT: none
// initiate windows and set up coordinate locations
{
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// background color is charcoal
	glClearColor (0.21, 0.27, 0.31, 0.0);

	// flip vertex locations
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	// define coordinate system in x, y 0-1
	gluOrtho2D (0.0, ww, 0.0, wh);

	// set default segment length to 20
	fractal.setLength(0.7);
	// set default line type to LINE_STRIP
	fractal.setLineType('s');
}

void initMenu ()
// INPUT: fractal object	OUTPUT: none
// initialize the menu on a right mouse click
{
	glutCreateMenu(menu);			// call the menu function
	glutAddMenuEntry("Create New (default)", 0);
	glutAddMenuEntry("Create New (custom)", 1);
	glutAddMenuEntry("Toggle Random Colors", 6);
	glutAddMenuEntry("Segment Length", 2);
	glutAddMenuEntry("Type of segments: Line Strip (default)", 7);
	glutAddMenuEntry("Type of segments: Lines", 8);
	glutAddMenuEntry("Clear", 3);
	glutAddMenuEntry("Redisplay", 4);
	glutAddMenuEntry("Quit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main (int argc, char** argv)
// INPUT: none	OUTPUT: none
// main function, calls other functions
{
	srand(time(NULL));
	// console instructions
	cout << "Right click for menu\nLeft click to iterate fractal\n";
	cout << "Toggling random colors on may make segments of fractal difficult to see\n";
	cout << "There is a 20 iteration limit\n";
	cout << "Changing type of segments is noticeable with longer segment lengths\n";
	cout << "Line Strip connects all segments drawn\nLines does not, only draws horizontal lines creating shutter effect\n";
	
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