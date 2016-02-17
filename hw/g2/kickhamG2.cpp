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


TODO: fix setInMenu
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
		void drawFractal();				// function to draw fractal
		bool getInMenu();				// return if left click happened in menu
		void setInMenu(bool);			// set inMenu value to true or false
	private:
		int levels;						// number of levels in the fractal
		bool inMenu;			// was left click in menu?
		vector<float> fractalPoints;	// vector of points in fractal
};

const int iterationLimit = 7;		// maximum number of iterations allowed
const float ww = 500.0;				// set window's width
const float wh = 500.0;				// set window's height
dragonFractal fractal;				// create an object to use

void dragonFractal::drawFractal()
// INPUT: none?	OUTPUT: none
// draw next fractal level
{
	cout << "Draw fractal\n";
}

void dragonFractal::setInMenu(bool value)
// INPUT: true or false bool value	OUTPUT: none
// set the value of in menu to true or false
{
	this->inMenu = value;
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
	// if the left mouse was pressed and it wasn't in the menu
	if (state == GLUT_UP && !fractal.getInMenu())
		fractal.drawFractal();
}

void menu (int menuVal)
// INPUT: value of menu option clicked	OUTPUT: none
// call function to do based on menu option chosen
{
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
}

void initMenu ()
// INPUT: fractal object	OUTPUT: none
// initialize the menu on a right mouse click
{
	fractal.setInMenu(true);		// say in menu
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