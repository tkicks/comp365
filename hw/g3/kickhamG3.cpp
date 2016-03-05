/*
Name:		Tyler Kickham
Program:	Five Flags Roller Coaster (kickhamG3.cpp)
Purpose:	Use C++ and OpenGL to create a graphic based on the data read
			in from a user dictated input file
Input:		The user will enter a file name in the terminal while starting
			the program. IE: kickhamG3 data.input
			The user can also use the keyboard to:
					0: display linear spline (rollercoaster track)
					1: display quadratic spline (rollercoaster track)
					2: display cubic (optional) spline (rollercoaster track)
					4: make the rollercoaster go faster
					6: make the rollercoaster go slower
					+: zoom in
					-: zoom out
					m: choose new data points with mouse (optional)
					n: display the next set of knots in the file (display
						error message if no more lines)
					r: reposition cart to beginning of track
					s: start moving the cart along the track
					q: quit
Output:		A computer graphic window will be displayed with a beginning
			block and an ending block along with a rollercoaster track as
			defined by the file being read in.  Upon various keypresses,
			the user will be able to change the shape of the track, view
			an animation of the cart moving along the track, change the
			speed of the cart, make everything bigger (zoom in) or smaller
			(zoom out).
*/

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class rollercoaster
{
	public:
		rollercoaster() {};
		~rollercoaster() {};
		void readCoaster(char *filename);			// read in the rollercoaster
		void drawCoaster();							// function to draw rollercoaster
		void drawLinear();							// draw the track linearly
		void setSpline(bool lin, bool quad);		// set which type of spline to use
	private:
		vector<vector <float> > knots;				// vector of coordinates for knots read in from file
		vector<float> tempVector;					// intermediary vector for 2d vector
		bool linear;								// true if using linear spline
		bool quadratic;								// true if using quadratic spline
};

const float ww = 1000.0;			// set window's width
const float wh = 800.0;				// set window's height
rollercoaster coaster;				// create an object to use


void rollercoaster::readCoaster(char *filename)
// INPUT: input file 	OUTPUT: none
// read the data in from the file
{
	knots.clear();
	float x, y;
	int numPoints = 0;
	int i = 0;
	// ifstream numberFile(filename.c_str());			// convert filename to chars to be opened
	// if (!numberFile.good())
	ifstream dataFile(filename);
	if (!dataFile.good())							//
		cout << "File not found.\n";				// see if file exists

	dataFile >> numPoints;

	while (x != 1000)								// while not past the last coordinate
	{
		dataFile >> x >> y;							// read in the next x and y values
		tempVector.push_back(x);					//
		tempVector.push_back(y);					// put the coordinates together
		knots.push_back(tempVector);				// store new coordinates
		tempVector.clear();							// empty out old vector
	}
}

void rollercoaster::drawCoaster()
// INPUT: none	OUTPUT: none
// draws rollercoaster
{
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(4.0);
	// starting gate
	glBegin(GL_LINE_STRIP);
		glVertex3f(-50.0, -150.0, 0.0);
		glVertex3f(-50.0, 150.0, 0.0);
		glVertex3f(0.0, 150.0, 0.0);
		glVertex3f(0.0, -150.0, 0.0);
		glVertex3f(-50.0, -150.0, 0.0);
	glEnd();
	// ending gate
	glBegin(GL_LINE_STRIP);
		glVertex3f(ww+50.0, -150.0, 0.0);
		glVertex3f(ww+50.0, 150.0, 0.0);
		glVertex3f(ww, 150.0, 0.0);
		glVertex3f(ww, -150.0, 0.0);
		glVertex3f(ww+50.0, -150.0, 0.0);
	glEnd();
	// hangers
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
		for (int i = 1; i < knots.size()-1; i++)
		{
			glVertex3f(knots[i][0], knots[i][1], 0.0);
			glVertex3f(knots[i][0], knots[i][1]+100, 0.0);
		}
	glEnd();
	// knots
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
		for (int i = 0; i < knots.size(); i++)
		{
			// cout << knots[i][0] << ", " << knots[i][1] << endl;
			glVertex3f(knots[i][0], knots[i][1], 0.0);
		}
	glEnd();
	// tracks
	// linear
	if (linear)
		coaster.drawLinear();

	glFlush();
}

void rollercoaster::drawLinear()
// INPUT: none	OUTPUT: none
// draws linear spline between the points
{
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < knots.size(); i++)
			glVertex3f(knots[i][0], knots[i][1], 0.0);
	glEnd();
}

void rollercoaster::setSpline(bool lin, bool quad)
// INPUT: bool value of linear spline, bool value of quadratic spline
// OUTPUT: none
// set which type of spline to use
{
	linear = lin;
	quadratic = quad;
}

void display ()
// INPUT: none	OUTPUT: none
// create the display
{
	// set up window
	glClear (GL_COLOR_BUFFER_BIT);

	coaster.drawCoaster();

	// draw
	glFlush ();
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
	gluOrtho2D (0.0-50, ww+50, -wh, wh);

	coaster.setSpline(true, false);		// init to linear spline
}

int main (int argc, char** argv)
// INPUT: none	OUTPUT: none
// main function, calls other functions
{
	
	// initiate window/viewport
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

	// size of window w/ const global (width, height)
	glutInitWindowSize (ww, wh); 

	// position in top left
	glutInitWindowPosition (50, 0);

	// make window w/ title
	glutCreateWindow ("Five Flags");

	// initialize
	init ();

	// console instructions
	cout << "List of keypresses\n";
	
	// read file
	char *filename = argv[1];
	coaster.readCoaster(filename);

	// continuously call display function
	glutDisplayFunc(display);

	// keep repeating
	glutMainLoop();
}