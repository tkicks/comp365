// colorInterpolate.fs
//
// Interpolates colors between red and green.

uniform float colorParamShader = 0.0; // Parameter to interpolate between two colors. 
                                      // (echoes the value of its counterpart variable in the program).

void main()
{
   // Linear interpolation between two color.
   gl_FragColor = vec4(1.0 - colorParamShader, colorParamShader, 0.0, 1.0); 
}
