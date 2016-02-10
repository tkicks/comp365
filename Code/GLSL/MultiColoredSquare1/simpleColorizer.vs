// simpleColorizer.vs
//
// Sets front and back color.

void main()
{	
   gl_FrontColor = gl_Color;
   gl_BackColor = vec4(1.0, 0.0, 0.0, 1.0);
   
   gl_Position = ftransform();
}
