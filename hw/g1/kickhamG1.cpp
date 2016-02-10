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
void toggleHighlights(int building);
void drawRoads();
void drawParkingLots();
void drawGrass();
void drawHighlights(int building);
void drawDorms();
void drawHaas();
void drawLibrary();
void drawOldSC();
void drawNewSC();

const int wh = 938;			// window height proportions based off of a 5x8 printout
const int ww = 1500;		// window width proportions based off of a 5x8 printout
bool labels = false;
bool roads = true;
bool highlight[5];


void display ()
{
   // set up window
   glClear (GL_COLOR_BUFFER_BIT);

   // draw the parking lots
   drawParkingLots();

   // draw the fields, dimple, and pond
   drawGrass();

   // draw the buildings that get highlighted
    for (int i = 0; i < 5; i++)
   		drawHighlights(i);


   // draw the roads
   drawRoads();
   
   // draw
   glFlush ();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key) {
		case 'c': toggleHighlights(0);
				  break;
		case 'd': toggleHighlights(1);
				  break;
		case 'f': toggleHighlights(2);
				  break;
		case 'l': toggleHighlights(3);
				  break;
		case 's': toggleHighlights(4);
				  break;
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
   glClearColor (0.0, 0.5, 0.0, 0.0);

   // flip vertex locations
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   
   // define coordinate system in x, y 0-1
   gluOrtho2D (0.0, ww, 0.0, wh);

   // say no buildings are highlighted
   for (int i = 0; i < 5; i++)
   	highlight[i] = false;
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

void toggleHighlights(int building)
// toggle whether or not a building is highlighted
// 0 = chapel, 1 = dorms, 2 = field house, 3 = library, 4 = science center
{
	if (highlight[building] == false)
		highlight[building] = true;
	else
		highlight[building] = false;

	switch(building) {
		case 0:	drawHighlights(building);
				break;
		case 1:	drawHighlights(building);
				break;
		case 2:	drawHighlights(building);
				break;
		case 3:	drawHighlights(building);
				break;
		case 4:	drawHighlights(building);
				break;
	}
}

void drawRoads()
// draws roads, maps out from West to East
// ww = window width, wh = window height
{
	if (roads == 1)
		// set road color to black
   		glColor3f(0.0, 0.0, 0.0);
   	else
   		// set road color to green for the grass
   		glColor3f(0.0, 0.5, 0.0);

	// Rt 123
	glRectf((.25*ww), wh, ww, (.98*wh));
	// Rt 123 slant
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
	glRectf((.2375*ww), wh, (.25*ww), (.4*wh));
	// Howard Street (East-West)
	glRectf((.1*ww), (.4*wh), (.275*ww), (.38*wh));

	// Fillmore Drive
	// Haas curve
	glLineWidth(4.0);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f((.42*ww), (.2*wh), 0.0);
		glVertex3f((.25*ww), (.35*wh), 0.0);
		glVertex3f((.5*ww), (.42*wh), 0.0);
		glVertex3f((.39*ww), (.3*wh), 0.0);

	glEnd();

	// East end
	glBegin(GL_POLYGON);
		glVertex3f((.72*ww), (.6*wh), 0.0);
		glVertex3f((.78*ww), (.65*wh), 0.0);
		glVertex3f((.78*ww), (.63*wh), 0.0);
		glVertex3f((.725*ww), (.59*wh), 0.0);
	glEnd();

	// Pine Street
	glRectf((.6875*ww), wh, (.7075*ww), (.9*wh));
	// Pine Street slant
	glBegin(GL_POLYGON);
		glVertex3f((.6875*ww), (.9*wh), 0.0);
		glVertex3f((.7075*ww), (.9*wh), 0.0);
		glVertex3f(ww, 0.0, 0.0);
		glVertex3f((.98*ww), 0.0, 0.0);
	glEnd();

	// Roads To Parking Lots
	// P10
	glRectf((.15*ww), (.95*wh), (.16*ww), (.9*wh));

	// P7
	glBegin(GL_POLYGON);
		glVertex3f((.495*ww), (.27*wh), 0.0);	// mid left
		glVertex3f((.469*ww), (.3*wh), 0.0);	// top left
		glVertex3f((.4715*ww), (.305*wh), 0.0);	// top right
		glVertex3f((.5*ww), (.28*wh), 0.0);		// mid right
		glVertex3f((.507*ww), (.25*wh), 0.0);	// bottom right
		glVertex3f((.5*ww), (.25*wh), 0.0);		// bottom left
	glEnd();

	// To Chase
	glRectf((.5875*ww), (.79*wh), (.65*ww), (.8*wh));
	// Chase slant
	glBegin(GL_POLYGON);
		glVertex3f((.65*ww), (.8007*wh), 0.0);
		glVertex3f((.731*ww), (.6*wh), 0.0);
		glVertex3f((.725*ww), (.59*wh), 0.0);
		glVertex3f((.645*ww), (.79*wh), 0.0);
	glEnd();

	// P3
	glBegin(GL_POLYGON);
		glVertex3f((.875*ww), (.35*wh), 0.0);
		glVertex3f((.88*ww), (.33*wh), 0.0);
		glVertex3f((.85*ww), (.26*wh), 0.0);
		glVertex3f((.845*ww), (.28*wh), 0.0);
	glEnd();

	// draw roads
	glFlush();
}

void drawParkingLots()
// draws parking lots from West to East
{
	// color parking lots grey
	glColor3f(0.5, 0.5, 0.5);

	// P10
	glRectf((.15*ww), (.9*wh), (.2*ww), (.77*wh));

	// P9
	glRectf((.325*ww), (.35*wh), (.344*ww), (.29*wh));

	// P8
	// top rectangle
	glRectf((.3*ww), (.22*wh), (.3875*ww), (.18*wh));
	// connector
	glRectf((.3*ww), (.22*wh), (.325*ww), (.15*wh));
	// bottom rectangle
	glRectf((.3*ww), (.15*wh), (.3875*ww), (.1*wh));

	// P7
	glRectf((.4875*ww), (.25*wh), (.51*ww), (.18*wh));

	// P4
	// main lot
	glRectf((.575*ww), (.6*wh), (.65*ww), (.54*wh));
	// lower lot
	glRectf((.6125*ww), (.54*wh), (.65*ww), /*(.46*wh)*/(.5*wh));

	// P3
	// soccer fields going clockwise
	glBegin(GL_POLYGON);
		glVertex3f((.76*ww), (.35*wh), 0.0);	// bottom left before connector
		glVertex3f((.693*ww), (.4*wh), 0.0);	// farthest left
		glVertex3f((.7*ww), (.42*wh), 0.0);
		glVertex3f((.75*ww), (.38*wh), 0.0);
		glVertex3f((.78*ww), (.52*wh), 0.0);
		glVertex3f((.8*ww), (.5*wh), 0.0);		// lower bump from right
		glVertex3f((.78*ww), (.42*wh), 0.0);
		glVertex3f((.8*ww), (.405*wh), 0.0);
		glVertex3f((.78*ww), (.35*wh), 0.0);	// bottom right before connector
	glEnd();
	// connector between lots
	glBegin(GL_POLYGON);
		glVertex3f((.78*ww), (.35*wh), 0.0);
		glVertex3f((.845*ww), (.28*wh), 0.0);
		glVertex3f((.82*ww), (.28*wh), 0.0);
		glVertex3f((.76*ww), (.35*wh), 0.0);
	glEnd();
	// main lot
	glBegin(GL_POLYGON);
		glVertex3f((.837*ww), (.31*wh), 0.0);	// top right
		glVertex3f((.875*ww), (.16*wh), 0.0);	// bottom right
		glVertex3f((.809*ww), (.09*wh), 0.0);	// bottom left
		glVertex3f((.76*ww), (.24*wh), 0.0);	// top left
	glEnd();

	// draw
	glFlush();
}

void drawGrass()
// draws fields, dimple, and pond from West to East
{
	// make grass a dark green
	glColor3f(0.0, 0.2, 0.0);

	// Dimple
	glRectf((.35*ww), (.82*wh), (.4*ww), (.58*wh));

	// Chapel Field
	glRectf((.475*ww), (.8*wh), (.5*ww), (.68*wh));

	// Peacock Pond

	// Turf Field
	glBegin(GL_POLYGON);
		glVertex3f((.532*ww), (.12*wh), 0.0);	// top left
		glVertex3f((.6125*ww), (.1*wh), 0.0);	// top right
		glVertex3f((.6*ww), 0.0, 0.0);
		glVertex3f((.5125*ww), 0.0, 0.0);
	glEnd();

	// Women's Soccer Field
	glBegin(GL_POLYGON);
		glVertex3f((.656*ww), (.32*wh), 0.0);	// top right
		glVertex3f((.72*ww), (.22*wh), 0.0);	// bottom right
		glVertex3f((.675*ww), (.15*wh), 0.0);
		glVertex3f((.6125*ww), (.25*wh), 0.0);
	glEnd();

	// Men's Soccer Field
	glBegin(GL_POLYGON);
		glVertex3f((.735*ww), (.55*wh), 0.0);	// top left
		glVertex3f((.775*ww), (.52*wh), 0.0);	// top right
		glVertex3f((.745*ww), (.39*wh), 0.0);
		glVertex3f((.7*ww), (.425*wh), 0.0);
	glEnd();
}

void drawHighlights(int building)
// draw the highlighted buildings from West to East
{
	if (highlight[building] == true)
		glColor3f(1.0, 1.0, 1.0);		// set color to white
	else
		glColor3f(.55, .27, .07);		// set color to brown
	switch(building) {
		// (un)highlight chapel
		case 0:	glRectf((.407*ww), (.77*wh), (.444*ww), (.74*wh));
				break;
		// (un)highlight dorms
		case 1:	drawDorms();
				break;
		// (un)highlight field house (HAAS)
		case 2: drawHaas();
				break;
		case 3: drawLibrary();
				break;
		case 4: drawOldSC();
				drawNewSC();
				break;
		default: break;
	}

	// draw building
	glFlush();
}

void drawDorms()
// draws dorms
{
	// Beard
	glRectf((.225*ww), (.36*wh), (.275*ww), (.32*wh));
	// Kilham
	glRectf((.2625*ww), (.95*wh), (.281*ww), (.88*wh));
	glRectf((.2625*ww), (.95*wh), (.315*ww), (.92*wh));
	// Metcalf
	glRectf((.3125*ww), (.95*wh), (.331*ww), (.88*wh));
	// Chapin
	glRectf((.2813*ww), (.85*wh), (.312*ww), (.82*wh));
	// Larcom
	glRectf((.314*ww), (.82*wh), (.331*ww), (.79*wh));
	// Emerson dorms
	glRectf((.2625*ww), (.78*wh), (.313*ww), (.74*wh));
	// Everett
	glRectf((.2625*ww), (.74*wh), (.281*ww), (.61*wh));
	glRectf((.2625*ww), (.61*wh), (.3*ww), (.58*wh));
	// Keefe
	glRectf((.2625*ww), (.48*wh), (.2813*ww), (.42*wh));
	// Gebbie
	glRectf((.282*ww), (.49*wh), (.325*ww), (.52*wh));
	// Cragin
	glRectf((.325*ww), (.68*wh), (.345*ww), (.64*wh));
	// Stanton
	glRectf((.313*ww), (.61*wh), (.328*ww), (.56*wh));
	// Clark
	glRectf((.575*ww), (.92*wh), (.59*ww), (.88*wh));
	// Mac
	glRectf((.61*ww), (.88*wh), (.625*ww), (.83*wh));
	// Young
	glRectf((.6375*ww), (.92*wh), (.653*ww), (.88*wh));
	// Meadows North
	glRectf((.6125*ww), (.72*wh), (.656*ww), (.695*wh));
	// Meadows Center
	glRectf((.6125*ww), (.72*wh), (.63*ww), (.64*wh));
	// Meadows East/West
	glRectf((.594*ww), (.64*wh), (.656*ww), (.61*wh));
}

void drawHaas()
// draw Haas (un)highlighted
{
	// Westernmost square
	glRectf((.406*ww), (.2*wh), (.4375*ww), (.15*wh));
	// connector to larger building
	glRectf((.406*ww), (.19*wh), (.4875*ww), (.16*wh));
	// top main building
	glRectf((.444*ww), (.22*wh), (.4875*ww), (.19*wh));
	// bottom main building
	glRectf((.444*ww), (.18*wh), (.5125*ww), (.12*wh));
}

void drawLibrary()
// draw (un)highlighted library
{
	// doorway
	glRectf((.3625*ww), (.56*wh), (.3875*ww), (.55*wh));
	// main building
	glRectf((.354*ww), (.55*wh), (.397*ww), (.5*wh));
}

void drawOldSC()
// draw (un)highlighted old science center
{
	// Kollet Center (second wh is top of P9)
	glRectf((.325*ww), (.44*wh), (.344*ww), (.35*wh));
	// center of old SC
	glRectf((.344*ww), (.4*wh), (.376*ww), (.29*wh));
	// bottom of old SC
	glRectf((.362*ww), (.29*wh), (.376*ww), (.28*wh));
	// top of old SC
	glRectf((.36*ww), (.41*wh), (.376*ww), (.4*wh));
}

void drawNewSC()
// draw (un)highlighted new science center
{
	//
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