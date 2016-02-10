// passThrough.fs
//
// Pass-through fragment shader: fragment color is set simply 
// to gl_color, which is computed from the values of 
// gl_FrontColor and gl_BackColor in the vertex shader, 
// depending on which side of the primitive is visible,
// and interpolated.

void main()
{
   gl_FragColor = gl_Color;
}
