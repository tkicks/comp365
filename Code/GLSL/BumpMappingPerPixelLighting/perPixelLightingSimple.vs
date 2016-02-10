// perPixelLightingSimple.vs
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
   // The following set of equations is copied from perVertexLightingSimple.vs. See comments there.
   // The variables normal, lightDirection and halfVector are passed as varyings to 
   // the fragment shader for interpolation across interior pixels.
   normal = normalize(gl_NormalMatrix * gl_Normal);
   lightDirection = normalize(gl_LightSource[0].position.xyz);
   halfVector = normalize(gl_LightSource[0].halfVector.xyz);     
   
   // The following set is copied as well from phongLightingSimple.vs.
   // The variables emission, globalAmbient and ambient are passed as varyings
   // to the fragment shader, where they will simply be interpolated across interior
   // pixels. Nothing more need be done with these three light components, which are 
   // exactly the same as in the case of per-vertex lighting.
   emission = gl_FrontMaterial.emission;
   globalAmbient =  gl_LightModel.ambient * gl_FrontMaterial.ambient;
   ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;   
   
   // The following two equations pass the light's diffuse and specular color intensities, 
   // respectively scaled by the material's corresponding color values, as varyings 
   // to the fragment shader for interpolation across interior pixels.
   // The fragment shader will further attenuate them *per-pixel*.
   diffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
   specular = gl_LightSource[0].specular * gl_FrontMaterial.specular;
   
   gl_Position = ftransform();
}