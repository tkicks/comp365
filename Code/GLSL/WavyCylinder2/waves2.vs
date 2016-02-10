// waves2.vs
//
// Creates waves on a cylinder.

attribute float scaleFactorShader = 1.0; // Parameter to scale the circular cross-section of the cylinder
                                         // (echoes the value of its counterpart variable in the program).

void main()
{	
   const float PI = 3.14159265358979324;
   vec4 v = vec4(gl_Vertex);
   
   // Next two statements create a wave on the surface of the cylinder, obtained
   // by scaling its circular cross-section by a scale factor (calculated in the
   // program) which follows a sine function along the z-axis. 
   v.x *= scaleFactorShader;
   v.y *= scaleFactorShader;
   
   gl_Position = gl_ModelViewProjectionMatrix * v;
}
