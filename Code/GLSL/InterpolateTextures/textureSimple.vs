// textureSimple.vs
//
// Assigns vertex texture coordinates.

void main() 
{
   // Store texture coordinates declared in the program.
   gl_TexCoord[0] = gl_MultiTexCoord0; 
   gl_TexCoord[1] = gl_MultiTexCoord0;
   
   gl_Position = ftransform();
} 

