// positionToColor.fs
//
// Fragment colors are defined using vertex positions.

varying vec3 vertexColor; // Varying variable to receive vertex position (as color) from vertex shader.

void main()
{
   gl_FragColor = vec4(vertexColor, 1.0); // Fragment color is set to color computed from position
                                          // by vertex shader.
}
