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
		char getSpline();							// return which type of spline is used
		float findSlope(float x1, float y1, float x2, float y2);	// find slope of line at current point
		float findYInt(float x, float y, float slope);				// find the y-intercept of the line
		float getWhichKnot(float cartX);			// return which knot iteration is being used to figure out slope/intercept pair
		void reset();								// reset the coaster

		void drawCart();							// draw the rollercoaster cart
		float getCartY();							// figure out the next y value of the cart
		void animate();								// animate cart
	protected:
		vector<vector <float> > knots;				// vector of coordinates for knots read in from file
		vector<float> tempVector;					// intermediary vector for 2d vector
		bool linear;								// true if using linear spline
		bool quadratic;								// true if using quadratic spline

		float cartX, cartY;							// x and y coordinates
		vector<vector <float> > slopeIntercepts;	// vector holding slopes and intercepts for each piece of spline

};

const float ww = 1000.0;			// set window's width
const float wh = 800.0;				// set window's height
float xMin = 0.0;
float yMin = 0.0;
float xMax = ww;
float yMax = wh;
long waitTime = 75000;				// time to wait between frames in animation
rollercoaster coaster;				// create rollercoaster object to use


static void Idle( void )

/* This is used to slow down your animation, if necessary.  I used it as
 * a debugging tool - see the commented out parts.  I also used it to 
 * change the z value used in the animation.
 */

{
	clock_t endWait;  /* end of wait interval */

	/* a better way to wait: */
	endWait = clock () + waitTime;
	/* please wait...*/
	while (clock () < endWait);
	glutPostRedisplay();
	// coaster.drawCoaster();
	// coaster.drawCart();
}

void rollercoaster::readCoaster(char *filename)
// INPUT: input file 	OUTPUT: none
// read the data in from the file
{
	knots.clear();
	float x, y;
	int numPoints = 0;
	int i = 0;
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
			glVertex3f(knots[i][0], knots[i][1], 0.0);
	glEnd();
	// tracks
	// linear
	if (linear)
		coaster.drawLinear();

	coaster.drawCart();
	glFlush();
}

void rollercoaster::drawCart()
// INPUT: none	OUTPUT: none
// init x and y coords to 0,0 and draw cart at x, y
{
	// cart support wire
	glBegin(GL_LINE_STRIP);
		glVertex3f(cartX, cartY, 0.0);
		glVertex3f(cartX, cartY - 25.0, 0.0);
	glEnd();
	// cart
	glRectf(cartX - 25, cartY - 25, cartX + 25, cartY - 50);
	
	glFlush();
}

float rollercoaster::getCartY()
// INPUT: none	OUTPUT: next y-coordinate for cart animation
// return the next y-coord for the cart's animation
{
	if (coaster.getSpline() == 'l')
	{
		float whichKnot = coaster.getWhichKnot(cartX);	// which iteration for slope/intercept vector
		float slope = slopeIntercepts[whichKnot][0];
		float yInt = slopeIntercepts[whichKnot][1];
		cartY = slope * cartX + yInt;
		return cartY;
	}
}

void rollercoaster::animate()
// INPUT: none	OUTPUT: none
// animates the cart by moving x and y coords then redrawing
{
	if (linear)
	{
		// find the slopes and y-intercepts for each and store in slopeIntercepts
		for (int i = 0; i < knots.size()-1; i++)
		{		
			tempVector.clear();
			float x1 = knots[i][0];
			float y1 = knots[i][1];
			float x2 = knots[i+1][0];
			float y2 = knots[i+1][1];
			float slope = findSlope(x1, y1, x2, y2);
			float yIntercept = findYInt(x1, y1, slope);
			tempVector.push_back(slope);
			tempVector.push_back(yIntercept);
			slopeIntercepts.push_back(tempVector);
		}
		while (cartX < 1000)
		{
			if (coaster.getSpline() == 'l')
			{
				cartX += 10;
				cartY = getCartY();

				// clear screen for animation
				glClear (GL_COLOR_BUFFER_BIT);
				glFlush();

				drawCoaster();
				drawCart();

				Idle();
				// cin.ignore();	// while testing
			}
		}
	}
}

float rollercoaster::findSlope(float x1, float y1, float x2, float y2)
// INPUT: 2 sets of float x, y coordinates 	OUTPUT: float value of slope
// return the slope of the line
{
	float y = y2 - y1;
	float x = x2 - x1;
	float slope = y/x;
	return slope;
}

float rollercoaster::findYInt(float x, float y, float slope)
// INPUT: x, y coords of first point in line, slope of line
// OUTPUT: y-intercept of the line
{
	float part1 = x * slope;
	float yInt = y - part1;
	return yInt;
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

char rollercoaster::getSpline()
// INPUT: none	OUTPUT: char first letter of type of spline
// return the first letter of the type of spline being used
{
	if (linear)
		return 'l';
	else
		return 'q';
}

float rollercoaster::getWhichKnot(float cartX)
// INPUT: cart x location 	OUTPUT: return which knot is being used
// return which knot to figure out which slope/intercept pair to use
{
	for (int i = 0; i < knots.size(); i++)
	{
		if (cartX <= knots[i][0])
			return i-1;
	}
}

void rollercoaster::reset()
// INPUT: none	OUTPUT: none
// reset the rollercoaster
{
	slopeIntercepts.clear();
	cartX = 0.0;
	cartY = 0.0;

	glClear (GL_COLOR_BUFFER_BIT);
	coaster.drawCoaster();
	coaster.drawCart();
	glFlush();
}

void display ()
// INPUT: none	OUTPUT: none
// create the display
{
	// set up window
	glClear (GL_COLOR_BUFFER_BIT);
	// glLoadIdentity();
	// gluLookAt(0.0, 0.0, viewZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// glScalef(1.0, 1.0, 1.0);
	
	coaster.drawCoaster();
	coaster.drawCart();

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
}

void keyboard(unsigned char key, int x, int y)
// INPUT: char of key pressed, current x, y locations	OUTPUT: none
// determine what to do based on key pressed
{
	switch(key) {
		case '0': coaster.setSpline(true, false);	// linear spline
				  coaster.drawCoaster();
				  break;
		case '4': waitTime += 5000;
				  glutPostRedisplay();
				  break;
		case '6': waitTime -= 5000;
				  glutPostRedisplay();
				  break;
		case '+': xMin -= ww*.1;
				  yMin -= wh*.1;
				  xMax += ww*.1;
				  yMax += wh*.1;
				  glViewport(xMin, yMin, xMax, yMax);
				  glutPostRedisplay();
				  break;
		case '-': xMin += ww*.1;
				  yMin += wh*.1;
				  xMax -= ww*.1;
				  yMax -= wh*.1;
				  glViewport(xMin, yMin, xMax, yMax);
				  glutPostRedisplay();
				  break;
		case 'r': coaster.reset();
				  break;
		case 's': coaster.animate();				// start the animation
				  break;
		case 27:	// esc
		case 03:	// ctrl+c
		case 'q': exit(1);							// quit
	}
}

// void reshape (int w, int h)
// {
//    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
//    /* set up matrices for projection coordinate system */
//    glMatrixMode (GL_PROJECTION);
//    glLoadIdentity ();
//    glFrustum (-1, 1, -1, 1, 1.5, 20.0);

//    /* reset matrices to user's coordinate system */
//    glMatrixMode (GL_MODELVIEW);
// }

int main (int argc, char** argv)
// INPUT: none	OUTPUT: none
// main function, calls other functions
{
	
	// initiate window/viewport
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

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

	// glutReshapeFunc(reshape);

	// continuously call keyboard function
	glutKeyboardFunc(keyboard);

	// keep repeating
	glutMainLoop();
}