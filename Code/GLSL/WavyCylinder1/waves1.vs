// waves1.vs
//
// Creates waves on a cylinder.

uniform float waveParamShader = 2.0; // Parameter to control waviness of cylinder surface 
                                     // (echoes the value of its counterpart variable in the program).

void main()
{	
   const float PI = 3.14159265358979324;
   vec4 v = vec4(gl_Vertex);
   
   // Next two statements create a wave on the surface of the cylinder, obtained
   // by multiplying its circular cross-section by a scale factor which follows
   // a sine function along the z-axis. 
   v.x *= 1 + 0.1 * sin(waveParamShader * PI * v.z);
   v.y *= 1 + 0.1 * sin(waveParamShader * PI * v.z);
   
   gl_Position = gl_ModelViewProjectionMatrix * v;
}
