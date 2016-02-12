/*
Name:		Tyler Kickham
Program:	Interactive Wheaton College campus map
Purpose:	Use C++ and OpenGL to create an interactive map of the Wheaton College campus
Input:		The user will input using the keyboard
			to: c - highlight the chapel
				d - highlight all dorms
				f - highlight the field house
				l - highlight the library
				s - highlight Mars Science Center
				R - toggle roads on/off
				L - toggle labels on/off
			or left-click the mouse to bring up a menu with the same options to click on
Output:		A computer graphic window will be displayed which will show the Wheaton College
			campus map, drawn with buildings, roads, and geographical features.
			Labels will be available to be toggled on/off.
			See input for list of interactions with map which will change the output.
			Menu functionality buggy. Mouse click "erases" labels. SC (un)highlights all buildings
			that wouldn't otherwise be (un)highlighted upon second click of mouse.
*/

#include <stdio.h>
#include <GL/glut.h>
#include <iostream>
GLenum rgb;					// for tkmap.c
#include "tkmap.c"
using namespace std;

class WheatonCollege
{
	public:
		WheatonCollege() {};
		~WheatonCollege() {};
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
		void drawBuildings();
		void writeLabels();
		void roadLabels();
		void buildingLabels();
		void adminLabels();
		void otherLabels();
		void writeWords(float x, float y, const char label[]);
		void checkToggle(int index);
};

void menu(int menuVal);

const int wh = 938;			// window height proportions based off of a 5x8 printout
const int ww = 1500;		// window width proportions based off of a 5x8 printout
bool labels = false;		// states whether labels are displayed
bool roads = true;			// states whether roads are displayed
bool highlight[5];			// array of bools for each possible highlighted location

// curve data
static float u = 1.0;		// curve parameter for Fillmore Dr
// coordinates for making the curve of Fillmore Dr
static float fillmorePoints[27][3] =
{
	{(.27*ww), (.39*wh), 0.0}, {(.34*ww), (.1*wh), 0.0}, {(.48*ww), (.32*wh), 0.0},
	{(.27*ww), (.4*wh), 0.0}, {(.34*ww), (.1*wh), 0.0}, {(.48*ww), (.324*wh), 0.0},
	{(.274*ww), (.401*wh), 0.0}, {(.34*ww), (.1*wh), 0.0}, {(.48*ww), (.327*wh), 0.0},
	{(.48*ww), (.327*wh), 0.0}, {(.5125*ww), (.42*wh), 0.0}, {(.5875*ww), (.48*wh), 0.0},
	{(.477*ww), (.317*wh), 0.0}, {(.5125*ww), (.42*wh), 0.0}, {(.5875*ww), (.47*wh), 0.0},
	{(.477*ww), (.316*wh), 0.0}, {(.5125*ww), (.42*wh), 0.0}, {(.5875*ww), (.476*wh), 0.0},
	{(.5875*ww), (.479*wh), 0.0}, {(.69*ww), (.5*wh), 0.0}, {(.725*ww), (.6*wh), 0.0},
	{(.5875*ww), (.476*wh), 0.0}, {(.69*ww), (.5*wh), 0.0}, {(.725*ww), (.592*wh), 0.0},
	{(.5875*ww), (.471*wh), 0.0}, {(.69*ww), (.5*wh), 0.0}, {(.725*ww), (.591*wh), 0.0}
};


void display ()
// INPUT: none	OUTPUT: none
// create the display
{
	WheatonCollege wheaton;			// instantiate a class object
	// set up window
    glClear (GL_COLOR_BUFFER_BIT);

	// draw the parking lots
    wheaton.drawParkingLots();

   	// draw the fields, dimple, and pond
    wheaton.drawGrass();

   	// draw the buildings that get highlighted
    for (int i = 0; i < 5; i++)
   		wheaton.drawHighlights(i);

   	// draw the buildings that don't get highlighted
   	wheaton.drawBuildings();

   	// draw the roads
   	wheaton.drawRoads();
   
   	// draw
   	glFlush ();
}

void keyboard(unsigned char key, int x, int y)
// INPUT: char of key pressed, current x, y locations	OUTPUT: none
// determine what to do based on key pressed
{
	WheatonCollege wheaton;			// instantiate a class object
	switch(key) {
		case 'c': wheaton.toggleHighlights(0);	// toggle chapel
				  break;
		case 'd': wheaton.toggleHighlights(1);	// toggle dorms
				  break;
		case 'f': wheaton.toggleHighlights(2);	// toggle field house
				  break;
		case 'l': wheaton.toggleHighlights(3);	// toggle library
				  break;
		case 's': wheaton.toggleHighlights(4);	// toggle science center
				  break;
		case 'R': wheaton.toggleRoads();		// toggle roads
				  break;
		case 'L': wheaton.toggleLabels();		// toggle labels
				  break;
		case 27:	// esc
		case 03:	// ctrl+c
		case 'q': exit(1);						// quit
	}
}

void init ()
// INPUT: none	OUTPUT: none
// initiate windows and set up coordinate locations
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

void WheatonCollege::toggleLabels()
// INPUT: none	OUTPUT: none
// toggles the labels on the map
{
	WheatonCollege wheaton;		// instantiate class object
	if (labels == true)			// if labels are on
		labels = false;			// turn off
	else						// otherwise
		labels = true;			// turn on
	wheaton.writeLabels();		// make labels
}

void WheatonCollege::toggleRoads()
// INPUT: none	OUTPUT: none
// toggles the roads on the map
{
	WheatonCollege wheaton;		// instantiate class object
	if (roads == true)			// if roads are on
	{
		roads = false;			// turn off
		labels = false;			// turn off labels (will only turn off road labels)
	}
	else						// otherwise
		roads = true;			// turn on
	wheaton.drawRoads();		// draw roads
}

void WheatonCollege::toggleHighlights(int building)
// INPUT: integer value for each building to be (un)highlighted
// 0 = chapel, 1 = dorms, 2 = field house, 3 = library, 4 = science center
// toggle whether or not a building is highlighted
{
	// if particular building isn't highlighted, highlight, vice versa
	if (highlight[building] == false)
		highlight[building] = true;
	else
		highlight[building] = false;

	// see which building just got (un)highlighted and draw it
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

void WheatonCollege::drawRoads()
// INPUT: none	OUTPUT: none
// draws roads, maps out from West to East
// ww = window width, wh = window height (global variables)
{
	if (roads)
		// set road color to black
   		glColor3f(0.0, 0.0, 0.0);
  
   	else
   		// set road color to green (same as background)
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
	glLineWidth(10.0);
	// glMap1f(target, t1, t2, stride, order, *control points)
	// target - 1 means dimension, 3 means x, y, z coordinates
	// t1, t2 - endpoints 
	// stride - num floats between start for one control point and next
	// order - num control points (coordinates in array)
	// *cp - array of control points

	// Haas curve (3 times to get depth to lines)
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[3][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[6][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));
	// After Haas curve
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[9][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[12][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[15][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));
	// P4 curve
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[18][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[21][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3.0, 3.0, &fillmorePoints[24][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glMapGrid1f(100, 0.0, 1.0);
	glEvalMesh1(GL_LINE, 0, (int)(u*100));


	// East end
	glBegin(GL_POLYGON);
		glVertex3f((.72*ww), (.6*wh), 0.0);
		glVertex3f((.78*ww), (.65*wh), 0.0);
		glVertex3f((.78*ww), (.63*wh), 0.0);
		glVertex3f((.725*ww), (.59*wh), 0.0);
	glEnd();

	// Pine Street
	glRectf((.6875*ww), wh, (.7075*ww), (.86*wh));
	// Pine Street slant
	glBegin(GL_POLYGON);
		glVertex3f((.6875*ww), (.86*wh), 0.0);
		glVertex3f((.7075*ww), (.86*wh), 0.0);
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

void WheatonCollege::drawParkingLots()
// INPUT: none	OUTPUT: none
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
	glBegin(GL_TRIANGLES);
		glVertex3f((.6125*ww), (.5*wh), 0.0);
		glVertex3f((.6125*ww), (.48*wh), 0.0);
		glVertex3f((.65*ww), (.5*wh), 0.0);
	glEnd();

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

void WheatonCollege::drawGrass()
// INPUT: none	OUTPUT: none
// draws fields, dimple, and pond from West to East
{
	// make grass a dark green
	glColor3f(0.0, 0.2, 0.0);

	// Dimple
	glRectf((.35*ww), (.82*wh), (.4*ww), (.58*wh));

	// Chapel Field
	glRectf((.475*ww), (.8*wh), (.502*ww), (.68*wh));

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

	glColor3f(0.0, 0.0, 0.7);
	// Peacock Pond
	glRectf((.533*ww), (.88*wh), (.55*ww), (.76*wh));
	// top
	float width = .5445;
	for (int i = 0; i < 2; i++)
	{
		glTranslatef((width*ww), (.89*wh), 0.0);			// put pond in the right spot
		gluDisk(gluNewQuadric(), 0.0, (.03*wh), 100, 1);	// draw north side of pond
		glTranslatef((-width*ww), (-.89*wh), 0.0);			// realign map
		width += .007;
	}
	// bottom
	width = .54;
	for (int i = 0; i < 4; i++)
	{
		glTranslatef((width*ww), (.73*wh), 0.0);			// put pond in the right spot
		gluDisk(gluNewQuadric(), 0.0, (.034*wh), 100, 1);	// draw south side of pond
		glTranslatef((-width*ww), (-.73*wh), 0.0);			// realign map
		width += .01;
	}
}

void WheatonCollege::drawHighlights(int building)
// INPUT: integer depicting building id value	OUTPUT: none
// draw the highlighted buildings from West to East
{
	if (highlight[building])			// if building is highlighted
		glColor3f(1.0, 1.0, 1.0);		// set color to white
	else
		glColor3f(.55, .27, .07);		// set color to brown
	switch(building) {
		case 0:	glRectf((.407*ww), (.77*wh), (.444*ww), (.74*wh));	// (un)highlight chapel
				break;
		case 1:	drawDorms();	// (un)highlight dorms
				break;
		case 2: drawHaas();		// (un)highlight field house (HAAS)
				break;
		case 3: drawLibrary();	// (un)highlight library
				break;
		case 4: drawOldSC();	// (un)highlight old SC
				drawNewSC();	// (un)highlight new SC
				break;
		default: break;
	}

	// draw building
	glFlush();
}

void WheatonCollege::drawDorms()
// INPUT: none	OUTPUT: none
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
	glRectf((.26*ww), (.48*wh), (.2813*ww), (.42*wh));
	// Gebbie
	glRectf((.282*ww), (.49*wh), (.325*ww), (.52*wh));
	// Cragin
	glRectf((.325*ww), (.68*wh), (.345*ww), (.64*wh));
	// Stanton
	glRectf((.313*ww), (.61*wh), (.328*ww), (.56*wh));
	// Clark
	glRectf((.574*ww), (.92*wh), (.595*ww), (.88*wh));
	// Mac
	glRectf((.608*ww), (.88*wh), (.625*ww), (.83*wh));
	// Young
	glRectf((.6375*ww), (.92*wh), (.663*ww), (.88*wh));
	// Meadows North
	glRectf((.6125*ww), (.72*wh), (.656*ww), (.695*wh));
	// Meadows Center
	glRectf((.6125*ww), (.72*wh), (.63*ww), (.64*wh));
	// Meadows East/West
	glRectf((.594*ww), (.64*wh), (.656*ww), (.61*wh));
}

void WheatonCollege::drawHaas()
// INPUT: none	OUTPUT: none
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

void WheatonCollege::drawLibrary()
// INPUT: none	OUTPUT: none
// draw (un)highlighted library
{
	// doorway
	glRectf((.3625*ww), (.56*wh), (.3875*ww), (.55*wh));
	// main building
	glRectf((.354*ww), (.55*wh), (.397*ww), (.5*wh));
}

void WheatonCollege::drawOldSC()
// INPUT: none	OUTPUT: none
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

void WheatonCollege::drawNewSC()
// INPUT: none	OUTPUT: none
// draw (un)highlighted new science center
{
	glBegin(GL_POLYGON);
		glVertex3f((.3875*ww), (.48*wh), 0.0);	// top left
		glVertex3f((.4875*ww), (.47*wh), 0.0);	// top right
		glVertex3f((.49*ww), (.44*wh), 0.0);	// far bottom right
		glVertex3f((.4688*ww), (.44*wh), 0.0);	// mid bottom 
		glVertex3f((.3875*ww), (.4*wh), 0.0);	// bottom left
	glEnd();
}

void WheatonCollege::drawBuildings()
// INPUT: none	OUTPUT: none
// draw buildings from West-East
{
	// 5t
	glRectf((.07*ww), (.75*wh), (.095*ww), (.72*wh));
	// Ecco
	glRectf((.07*ww), (.68*wh), (.095*ww), (.65*wh));
	// Bookstore
	glRectf((.125*ww), (.69*wh), (.175*ww), (.62*wh));
	// Norton Med
	glRectf((.125*ww), (.56*wh), (.175*ww), (.42*wh));
	// some house
	glRectf((.1375*ww), (.24*wh), (.1625*ww), (.21*wh));
	// Public Safety
	// main building
	glRectf((.1375*ww), (.19*wh), (.1625*ww), (.16*wh));
	// entrance
	glRectf((.15*ww), (.16*wh), (.1625*ww), (.14*wh));
	// Art house
	glRectf((.169*ww), (.37*wh), (.182*ww), (.35*wh));
	// Counselling Center
	glRectf((.186*ww), (.37*wh), (.21*ww), (.35*wh));
	// Marshall Center
	glRectf((.2125*ww), (.95*wh), (.23*ww), (.92*wh));
	// Sem
	glRectf((.2125*ww), (.9*wh), (.23*ww), (.88*wh));
	// Global Ed
	glRectf((.2125*ww), (.85*wh), (.23*ww), (.82*wh));
	// Davis
	// garage
	glRectf((.215*ww), (.76*wh), (.22*ww), (.75*wh));
	// main house
	glRectf((.2125*ww), (.75*wh), (.23*ww), (.72*wh));
	// doorway
	glRectf((.22*ww), (.72*wh), (.227*ww), (.71*wh));
	// life house
	glRectf((.18*ww), (.71*wh), (.194*ww), (.69*wh));
	// white house
	glRectf((.2125*ww), (.63*wh), (.23*ww), (.6*wh));
	// MAAD house
	glRectf((.2125*ww), (.55*wh), (.23*ww), (.52*wh));
	// Park hall
	glRectf((.344*ww), (.9*wh), (.3625*ww), (.86*wh));
	// Emerson
	glRectf((.313*ww), (.78*wh), (.335*ww), (.69*wh));
	// Jewish house
	glRectf((.265*ww), (.55*wh), (.281*ww), (.53*wh));
	// Mary Lyon
	// street/campus entrances
	glRectf((.42*ww), (.935*wh), (.4375*ww), (.885*wh));
	// main building
	glRectf((.41*ww), (.92*wh), (.444*ww), (.9*wh));
	// Knapton
	// top of hill
	glRectf((.41*ww), (.85*wh), (.427*ww), (.79*wh));
	// bottom of hill
	glRectf((.427*ww), (.84*wh), (.444*ww), (.78*wh));
	// Admissions
	// doorway
	glRectf((.41*ww), (.71*wh), (.427*ww), (.65*wh));
	// building
	glRectf((.41*ww), (.71*wh), (.45*ww), (.665*wh));
	// Balfour
	glBegin(GL_POLYGON);
		glVertex3f((.415*ww), (.62*wh), 0.0);	// top left
		glVertex3f((.481*ww), (.62*wh), 0.0);	// top right
		glVertex3f((.494*ww), (.6*wh), 0.0);	// mid right
		glVertex3f((.4625*ww), (.55*wh), 0.0);	// bottom right
		glVertex3f((.4375*ww), (.58*wh), 0.0);	// bottom mid
		glVertex3f((.415*ww), (.58*wh), 0.0);	// bottom left
	glEnd();
	// Registrar
	glRectf((.407*ww), (.57*wh), (.42*ww), (.55*wh));
	// Watson
	glRectf((.45*ww), (.95*wh), (.484*ww), (.9*wh));	// main building
	glRectf((.45*ww), (.94*wh), (.5*ww), (.91*wh));		// practice rooms
	// Meneely
	glBegin(GL_POLYGON);
		glVertex3f((.456*ww), (.835*wh), 0.0);	// top right laser room
		glVertex3f((.456*ww), (.85*wh), 0.0);	// top left
		glVertex3f((.5063*ww), (.85*wh), 0.0);	// top right
		glVertex3f((.5063*ww), (.82*wh), 0.0);	// bottom right
		glVertex3f((.4813*ww), (.82*wh), 0.0);	// mid offices
		glVertex3f((.4875*ww), (.81*wh), 0.0);	// mid art rooms
		glVertex3f((.458*ww), (.8*wh), 0.0);	// mid left art rooms
		glVertex3f((.456*ww), (.805*wh), 0.0);	// end of art rooms
		glVertex3f((.45*ww), (.805*wh), 0.0);	// bottom laser cutter room
		glVertex3f((.45*ww), (.835*wh), 0.0);	// top left laser room
	glEnd();
	// Power Plant
	glRectf((.506*ww), (.62*wh), (.53125*ww), (.6*wh));
	// Locksmith
	glRectf((.506*ww), (.6*wh), (.515*ww), (.565*wh));
	// Chase
	glRectf((.5625*ww), (.84*wh), (.594*ww), (.82*wh));	// entrance
	glRectf((.5625*ww), (.82*wh), (.58*ww), (.78*wh));	// connector
	glRectf((.5625*ww), (.79*wh), (.594*ww), (.77*wh));	// square
	// round
	glTranslatef((.565*ww), (.835*wh), 0.0);			// put round in the right spot
	gluDisk(gluNewQuadric(), 0.0, (.017*wh), 100, 1);	// draw chase round
	glTranslatef((-.565*ww), (-.835*wh), 0.0);			// realign map
	// Peace House
	glRectf((.67*ww), (.89*wh), (.683*ww), (.86*wh));
	// Outdoors
	glBegin(GL_POLYGON);
		glVertex3f((.7625*ww), (.594*wh), 0.0);			// top
		glVertex3f((.775*ww), (.58*wh), 0.0);			// right
		glVertex3f((.762*ww), (.555*wh), 0.0);			// bottom
		glVertex3f((.75*ww), (.57*wh), 0.0);			// left
	glEnd();
	// TWAP
	glBegin(GL_POLYGON);
		glVertex3f((.8125*ww), (.45*wh), 0.0);			// top
		glVertex3f((.825*ww), (.42*wh), 0.0);			// top right
		glVertex3f((.813*ww), (.41*wh), 0.0);			// mid right
		glVertex3f((.8105*ww), (.41*wh), 0.0);			// mid left
		glVertex3f((.8*ww), (.44*wh), 0.0);				// top left
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f((.817*ww), (.42*wh), 0.0);			// mid left
		glVertex3f((.825*ww), (.4*wh), 0.0);			// bottom right
		glVertex3f((.8125*ww), (.39*wh), 0.0);			// bottom
		glVertex3f((.805*ww), (.405*wh), 0.0);			// bottom left
	glEnd();
	// Bears
	glBegin(GL_POLYGON);
		glVertex3f((.8375*ww), (.4*wh), 0.0);			// top
		glVertex3f((.844*ww), (.38*wh), 0.0);			// right
		glVertex3f((.835*ww), (.36*wh), 0.0);			// bottom
		glVertex3f((.823*ww), (.38*wh), 0.0);			// left
	glEnd();
}

void WheatonCollege::writeLabels()
// INPUT: none	OUTPUT: none
// write/erase labels
{
	WheatonCollege wheaton;
	
	wheaton.roadLabels();
	wheaton.buildingLabels();
	wheaton.adminLabels();
	wheaton.otherLabels();
}

void WheatonCollege::roadLabels()
// INPUT: none	OUTPUT: none
// create labels for roads
{
	WheatonCollege wheaton;			// instantiate class object
	// Road labels
	if (labels)										// if labels are on make white
		glColor3f(1.0, 1.0, 1.0);
	else if (labels == false and roads == true)		// if labels are off and roads are on make text match roads
		glColor3f(0.0, 0.0, 0.0);
	else											// otherwise make text green to match background
		glColor3f(0.0, 0.5, 0.0);

	wheaton.writeWords(.48, .985, "East Main St");
	wheaton.writeWords(.17, .385, "Howard St");
	wheaton.writeWords(.105, .5, "1");				// Taunton Ave
	wheaton.writeWords(.24, .7, "2");				// Howard
	wheaton.writeWords(.352, .2269, "Fillmore Dr");
	wheaton.writeWords(.815, .5, "4");				// Pine St
	// Parking lots
	if (labels == true)
		glColor3f(1.0, 1.0, 1.0);					// if labels are on make white
	else
		glColor3f(0.5, 0.5, 0.5);					// if labels are off make grey
	wheaton.writeWords(.16, .82, "Lot 10");
	wheaton.writeWords(.324, .33, "Lot 9");
	wheaton.writeWords(.33, .12, "Lot 8");
	wheaton.writeWords(.488, .21, "Lot 7");
	wheaton.writeWords(.6, .57, "Lot 4");
	wheaton.writeWords(.81, .2, "Lot 3");
}

void WheatonCollege::buildingLabels()
// INPUT: none	OUTPUT: none
// create labels for highlightable buildings
{
	WheatonCollege wheaton;						// instantiate class object
	wheaton.checkToggle(0);						// check if chapel is highlighted
	wheaton.writeWords(.41, .755, "Chapel");	// write width%, height%, label
	wheaton.checkToggle(1);						// check if dorms highlighted
	wheaton.writeWords(.24, .34, "Beard");		// write width%, height%, label
	wheaton.writeWords(.263, .93, "Kilham");	// write width%, height%, label
	wheaton.writeWords(.302, .93, "Metcalf");	// write width%, height%, label
	wheaton.writeWords(.283, .83, "Chapin");	// write width%, height%, label
	wheaton.writeWords(.315, .806, "Lar");		// write width%, height%, label
	wheaton.writeWords(.315, .795, "com");		// write width%, height%, label
	wheaton.writeWords(.27, .765, "Emerson");	// write width%, height%, label
	wheaton.writeWords(.27, .752, "House");		// write width%, height%, label
	wheaton.writeWords(.27, .59, "Everett");	// write width%, height%, label
	wheaton.writeWords(.26, .45, "Keefe");		// write width%, height%, label
	wheaton.writeWords(.29, .5, "Gebbie");		// write width%, height%, label
	wheaton.writeWords(.325, .667, "Cra");		// write width%, height%, label
	wheaton.writeWords(.325, .656, "gin");		// write width%, height%, label
	wheaton.writeWords(.312, .582, "Stan");		// write width%, height%, label
	wheaton.writeWords(.3121, .569, "ton");		// write width%, height%, label
	wheaton.writeWords(.574, .9, "Clark");		// write width%, height%, label
	wheaton.writeWords(.608, .86, "Mac");		// write width%, height%, label
	wheaton.writeWords(.6375, .9, "Young");		// write width%, height%, label
	wheaton.writeWords(.618, .71, "Meadows");	// write width%, height%, label
	wheaton.writeWords(.618, .7, "North");		// write width%, height%, label
	wheaton.writeWords(.595, .62, "West");		// write width%, height%, label
	wheaton.writeWords(.638, .62, "East");		// write width%, height%, label
	wheaton.checkToggle(2);						// check if HAAS highlighted
	wheaton.writeWords(.45, .16, "Field House");	// write width%, height%, label
	wheaton.checkToggle(3);						// check if library highlighted
	wheaton.writeWords(.36, .52, "Library");	// write width%, height%, label
	wheaton.checkToggle(4);						// check if SC highlighted
	wheaton.writeWords(.33, .38, "Science");	// write width%, height%, label
	wheaton.writeWords(.34, .36, "Center");		// write width%, height%, label
	wheaton.writeWords(.39, .455, "Science");	// write width%, height%, label
	wheaton.writeWords(.4, .435, "Center");		// write width%, height%, label
}

void WheatonCollege::adminLabels()
// INPUT: none	OUTPUT: none
// label administrative and academic buildings
{
	WheatonCollege wheaton;			// instantiate class object
	// if labels on, make text white, else make text brown
	if (labels)
		glColor3f(1.0, 1.0, 1.0);
	else
		glColor3f(.55, .27, .07);

	// write %width, %height, label
	wheaton.writeWords(.13, .67, "Bookstore");
	wheaton.writeWords(.127, .5, "Norton Med");
	wheaton.writeWords(.346, .89, "Park");
	wheaton.writeWords(.346, .87, "Hall");
	wheaton.writeWords(.315, .74, "Emer");
	wheaton.writeWords(.315, .73, "son");
	wheaton.writeWords(.427, .91, "3");
	wheaton.writeWords(.41, .81, "Knapton");
	wheaton.writeWords(.412, .69, "Admission");
	wheaton.writeWords(.42, .6, "Balfour");
	wheaton.writeWords(.46, .92, "Watson");
	wheaton.writeWords(.46, .83, "Meneely");
	wheaton.writeWords(.565, .83, "Chase");
}

void WheatonCollege::otherLabels()
// INPUT: none	OUTPUT: none
// make the rest of the labels
{
	WheatonCollege wheaton;		// instantiate class object
	// if labels on make text white, else make text green
	if (labels)
		glColor3f(1.0, 1.0, 1.0);
	if (!labels)
		glColor3f(0.0, 0.2, 0.0);

	// write %width, %height, label
	wheaton.writeWords(.36, .74, "Dimple");
	wheaton.writeWords(.475, .74, "Chapel");
	wheaton.writeWords(.48, .72, "Field");
	wheaton.writeWords(.55, .05, "Turf");
	wheaton.writeWords(.65, .25, "Women's");
	wheaton.writeWords(.66, .23, "Soccer");
	wheaton.writeWords(.72, .48, "Men's");
	wheaton.writeWords(.73, .46, "Soccer");

	// if labels off make text blue and "erase" pond label
	if (!labels)
		glColor3f(0.0, 0.0, 0.7);

	wheaton.writeWords(.53, .735, "Peacock Pond");

	// if labels off make text dark green and "erase" grass labels
	if (!labels)
		glColor3f(0.0, 0.5, 0.0);

	wheaton.writeWords(.85, .85, "1: Taunton Ave");
	wheaton.writeWords(.85, .81, "2: Howard St");
	wheaton.writeWords(.85, .77, "3: Mary Lyon");
	wheaton.writeWords(.85, .73, "4: Pine St");
}

void WheatonCollege::checkToggle(int index)
// INPUT: index of highlight array	OUTPUT: none
// check if a building is highlighted
{
	if (labels && highlight[index])	// if labels on and index is true
		glColor3f(0.0, 0.0, 0.0);	// make black
	// if labels and false, or no labels and true make white
	else if ((labels && !highlight[index]) or (!labels && highlight[index]))
		glColor3f(1.0, 1.0, 1.0);
	// if no labels and highlight is false
	else if (!labels && !highlight[index])
		glColor3f(.55, .27, .07);
}

void WheatonCollege::writeWords(float x, float y, const char label[])
// INPUT: x, y coordinates, char array of label 	OUTPUT: none
// writes each letter to the map
{
	int i = 0;						// iterator
	glRasterPos2i((x*ww), y*wh);	// position w/ x,y% coordinates
	// while more chars, write to map
	while (label[i] != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[i]);
		i++;
	}

	glFlush();	// draw
}

void initMenu()
// INPUT: none	OUTPUT: none
// initiate the menu on mouse click
{
	// create menu and add options to it
	glutCreateMenu(menu);						// call menu function
	glutAddMenuEntry("Highlight Chapel", 0);
	glutAddMenuEntry("Highlight dorms", 1);
	glutAddMenuEntry("Highlight field house", 2);
	glutAddMenuEntry("Highlight library", 3);
	glutAddMenuEntry("Highlight Science Center", 4);
	glutAddMenuEntry("Toggle roads", 5);
	glutAddMenuEntry("Toggle labels", 6);
	glutAddMenuEntry("Quit", 7);
	glutAttachMenu(GLUT_LEFT_BUTTON);			// attach menu to left click action
}

void menu (int menuVal)
// INPUT: value of menu option clicked	OUTPUT: none
// what each item in the menu does based on assigned value
{
	WheatonCollege wheaton;		// instantiate class object
	// toggle objects based on menu value selected
	switch (menuVal) {
		case 0: wheaton.toggleHighlights(0);
				  break;
		case 1: wheaton.toggleHighlights(1);
				  break;
		case 2: wheaton.toggleHighlights(2);
				  break;
		case 3: wheaton.toggleHighlights(3);
				  break;
		case 4: wheaton.toggleHighlights(4);
				  break;
		case 5: wheaton.toggleRoads();
				  break;
		case 6: wheaton.toggleLabels();
				  break;
		case 7:	exit(1);	
	}
}

int main (int argc, char** argv)
// INPUT: none	OUTPUT: none
// main function, calls other functions
{
	// console instructions
	cout << "'c' - highlight Chapel\n"
		 << "'d' - highlight dorms\n"
		 << "'f' - highlight field house\n"
		 << "'l' - highlight library\n"
		 << "'s' - highlight Science Center\n"
		 << "'R' - toggle roads on/off\n"
		 << "'L' - toggle labels on/off\n"
		 << "'q', esc, ctrl+c - quit program\n"
		 << "Labels on roads/highlighted buildings toggled off when road/building is toggled\n"
		 << "Left click to access menu\n"
		 << "Calling menu will erase labels, selecting Science Center and clicking again will highlight all buildings that wouldn't otherwise be highlighted\n";
	
	// initiate window/viewport
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

	// initialize menu
	initMenu();

	// continuously call keyboard function
	glutKeyboardFunc(keyboard);

	// continuously call display function
	glutDisplayFunc(display);

	// keep repeating
	glutMainLoop();
}