// positionToColor.vs
//
// Vertex position coordinates are used to define colors.

varying vec3 vertexColor; // Varying variable to pass vertex position (as color) to fragment shader.

void main()
{	
   vertexColor = 0.01 * gl_Vertex.xyz; // Vertex color is obtained as the first three values in the
                                       // gl_Vertex position vector, divided by 100 to bring to 
									   // within 0 to 1.
   gl_Position = ftransform();
}
