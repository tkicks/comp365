// perPixelLightingSimple.fs
//
// Implements Phong's shading model, also called per-pixel lighting.
// Lighting computation that was done all in the vertex shader in case
// of per-vertex lighting in the program bumpMappingPerVertexLighting.cpp 
// is now split over the vertex and fragment shaders.
// In particular, per-vertex computation involving normals moves now to the fragment shader,
// so as to use the interpolated normal values at each pixel. 

varying vec3 normal, lightDirection, halfVector;
varying vec4 emission, globalAmbient, ambient, diffuse, specular;

void main()
{
   vec3 normalPerPixel;
   vec4 diffusePerPixel, specularPerPixel;

   normalPerPixel = normalize(normal); // Interpolation over interior pixels may change the size of normals. 
                                       // Therefore, normalize again.
   
   // The lightDirection and halfVector variables are respectively constant over all vertices, given the  
   // assumptions of a single directional light source and infinite viewpoint. Therefore, the interpolated 
   // values of each are respectively the same as well, and so need not be normalized again.
   
   // The next two equations, respectively, attenuate the diffuse and specular values received from the 
   // vertex shader with a factor computed exactly as in the case of per-vertex lighting, *except* that
   // now the interpolated normal value is used at each pixel.
   diffusePerPixel = max(dot(normalPerPixel, lightDirection), 0.0) * diffuse;
   specularPerPixel = pow( max(dot(normalPerPixel, halfVector), 0.0), gl_FrontMaterial.shininess ) * specular;
   
   // The emission, globalAmbient and ambient components are the same as for per-vertex lighting.
   // Only the diffuse and specular components change as per-pixel lighting uses interpolated normal values
   gl_FragColor =  emission + globalAmbient + ambient + diffusePerPixel + specularPerPixel;
}