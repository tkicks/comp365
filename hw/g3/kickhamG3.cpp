/*
Name:		Tyler Kickham
Program:	Five Flags Roller Coaster (kickhamG3.cpp)
Purpose:	Use C++ and OpenGL to create a graphic based on the data read
			in from a user dictated input file
Input:		The user will enter a file name in the terminal while starting
			the program. IE: kickhamG3 data.input
			The user can also use the keyboard to:
					0: display linear spline (rollercoaster track)
					4: make the rollercoaster go faster
					6: make the rollercoaster go slower
					+: zoom in
					-: zoom out
					m: choose new data points with mouse (if the user
						selects a coordinate with a lower x value than
						the last or 300<y<-300, it will not be recorded)
					n: display the next set of knots in the file (display
						error message if no more lines)
					r: reposition cart to beginning of track
					s: start moving the cart along the track
					q: quit (also ctrl+c and esc)
Output:		A computer graphic window will be displayed with a beginning
			block and an ending block along with a rollercoaster track as
			defined by the file being read in.  Upon various keypresses,
			the user will be able to change the shape of the track, view
			an animation of the cart moving along the track, change the
			speed of the cart, make everything bigger (zoom in) or smaller
			(zoom out).  Does not have quadratic or cubic capabilities.
			Sometimes an error that sets beginning 0, 0 coordinate to
			0, random y.
*/

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

void display();

class rollercoaster
{
	public:
		rollercoaster() {fileLine = 0;};
		~rollercoaster() {};
		void readCoaster();			// read in the rollercoaster
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
		bool userCoords();							// return if the user selected to get mouse coordinates
		void toggleUserCoords(int i);				// toggle if choosing coords or not
		void addCoord(float x, float y);			// add mouse click coordinates (not x<0||x>1000||y<-300||y>300)
		void clearKnots();							// clear knots vector
		void setName(char *filename);				// set file name
		float getBackX();
	protected:
		vector<vector <float> > knots;				// vector of coordinates for knots read in from file
		vector<float> tempVector;					// intermediary vector for 2d vector
		bool linear;								// true if using linear spline
		bool quadratic;								// true if using quadratic spline
		bool userCoord;								// if the user selects to set coords w/ mouse
		char *file;									// file name
		int fileLine;								// line in file

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

void rollercoaster::setName(char *filename)
// INPUT: file name 	OUTPUT: none
// set file name in class
{
	file = filename;
}

void rollercoaster::readCoaster()
// INPUT: input file 	OUTPUT: none
// read the data in from the file
{
	float x, y;
	int numPoints = 0;
	int i = 0;
	string line;
	ifstream dataFile(file);
	if (!dataFile.good())							//
		cout << "File not found.\n";				// see if file exists

	while (!dataFile.eof() && i < fileLine)
	{
		getline(dataFile, line);
		i++;
	}

	if (!dataFile.eof())
	{
		knots.clear();
		dataFile >> numPoints;
		while (x != 1000)								// while not past the last coordinate
		{
			dataFile >> x >> y;							// read in the next x and y values
			tempVector.push_back(x);					//
			tempVector.push_back(y);					// put the coordinates together
			knots.push_back(tempVector);				// store new coordinates
			tempVector.clear();							// empty out old vector
		}
		display();
	}
	else
		cout << "end of file\n";


	fileLine += 1;
}

void rollercoaster::addCoord(float x, float y)
// INPUT: x, y mouse coords	OUTPUT: none
// add mouse coords to coaster
{
	if ((x >= 0 && x <= 1000) && (y >= -300 && y <= 300))
	{
		tempVector.push_back(x);
		tempVector.push_back(y);
		knots.push_back(tempVector);
		tempVector.clear();
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
	else if (quadratic)
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
	// coaster.drawCoaster();
	coaster.drawCart();
	glFlush();
}

bool rollercoaster::userCoords()
// INPUT: none	OUTPUT: if the user is selecting coordinates
// return if the user is selecting user coordinates
{
	return userCoord;
}

void rollercoaster::clearKnots()
// INPUT: none	OUTPUT: none
// clear knots
{
	knots.clear();
}

float rollercoaster::getBackX()
// INPUT: none	OUTPUT: last x coord user clicked
// return the last x coordinate to determine if click is valid
{
	return knots.back()[0];
}

void display ()
// INPUT: none	OUTPUT: none
// create the display
{
	// set up window
	glClear (GL_COLOR_BUFFER_BIT);

	if (coaster.userCoords())
	{
		glBegin(GL_LINES);
			glVertex3f(0.0, 300.0, 0.0);
			glVertex3f(1000.0, 300.0, 0.0);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(0.0, -300.0, 0.0);
			glVertex3f(1000.0, -300.0, 0.0);
		glEnd();
		glFlush();
	}
	
	if (coaster.getSpline() == 'l' || coaster.getSpline() == 'q')
		coaster.drawCoaster();
	coaster.drawCart();

	// draw
	glFlush ();
}

void rollercoaster::toggleUserCoords(int i)
// INPUT: none	OUTPUT: none
// toggle mouse input on/off
{
	if (i == 0)
	{
		if (userCoord)
			userCoord = false;
		else
			userCoord = true;
		display();
	}
	else
		userCoord = false;
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
		case '0': if (coaster.userCoords())
					coaster.addCoord(1000, 0);
				  coaster.toggleUserCoords(1);
				  coaster.setSpline(true, false);	// linear spline
				  // coaster.readCoaster();
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
		case 's': if (coaster.userCoords())
					coaster.addCoord(1000, 0);
				  coaster.toggleUserCoords(1);
				  coaster.animate();				// start the animation
				  break;
		case 'm': if (!coaster.userCoords())
				  {
					coaster.clearKnots();
					coaster.addCoord(0, 0);
				  }
				  if (coaster.userCoords())
				  	coaster.addCoord(1000, 0);
				  coaster.toggleUserCoords(0);
				  break;
		case 'n': coaster.toggleUserCoords(1);
				  coaster.readCoaster();
				  break;
		case 27:	// esc
		case 03:	// ctrl+c
		case 'q': exit(1);							// quit
	}
}

void mouse (int button, int state, int x, int y)
// INPUT: which button pressed on mouse, state of object, x/y coords of click
// OUTPUT: none
// get coordinates from mouse
{
	if (button == GLUT_RIGHT_BUTTON)
		return;
	// if the left mouse was pressed and it wasn't in the menu draw fractal
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && coaster.userCoords())
	{
		if (x > coaster.getBackX())
			coaster.addCoord(x, (wh-y)-300);
	}
}

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
	cout << "0: display linear spline (rollercoaster track)\n";
	cout << "4: make the rollercoaster go faster\n";
	cout << "6: make the rollercoaster go slower\n";
	cout << "+: zoom in\n";
	cout << "-: zoom out\n";
	cout << "m: choose new data points with mouse\n";
	cout << "n: display the next set of knots in the file\n";
	cout << "r: reposition cart to beginning of track\n";
	cout << "s: start moving the cart along the track\n";
	cout << "q: quit (also ctrl+c and esc)\n";
	
	// read file
	char *filename = argv[1];
	coaster.setName(filename);
	coaster.readCoaster();

	// continuously call display function
	glutDisplayFunc(display);

	// continuously call keyboard function
	glutKeyboardFunc(keyboard);

	// continuously call mouse function
	glutMouseFunc(mouse);

	// keep repeating
	glutMainLoop();
}