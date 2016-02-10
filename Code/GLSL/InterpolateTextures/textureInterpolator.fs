// textureInterpolator.fs
//
// Interpolates between day and night sky textures

uniform sampler2D skyTexture; // Handle to access sky texture.
uniform sampler2D nightSkyTexture; // Handle to access night sky texture.
uniform float paramShader = 0.0; // Parameter to interpolate between two textures 
                                            // (echoes the value of its counterpart variable in the program).	

void main()
{
   // Linear interpolation between two textures.
   gl_FragColor = (1.0 - paramShader) * texture2D(skyTexture, gl_TexCoord[0].st) + 
                   paramShader * texture2D(nightSkyTexture, gl_TexCoord[1].st);
}

