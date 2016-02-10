// perVertexLightingSimple.vs
//
// Implements the OpenGL (Phong's) lighting equation assuming only one directional light source (therefore, no distance 
// attenuation or spotlight effect). Moreover, an infinite viewpoint is assumed. Only front face colors are computed.
// The colors computed at the vertices are simply interpolated by the pass-through fragment shader. 
// This is per-vertex lighting.

void main()
{
   vec3 normal, lightDirection, halfVector;
   vec4 emission, globalAmbient, ambient, diffuse, specular;

   normal = normalize(gl_NormalMatrix * gl_Normal); // Transform the normal as asked by the modelview transformations by 
                                                    // multiplying by the normal matrix, but this may change its length. 
													// Therefore, normalize again.

   lightDirection = normalize(gl_LightSource[0].position.xyz); // As the given light source is directional, position.w = 0
                                                               // is disregarded. Morover, because the light source is 
                                                               // directional, its direction is same for all vertices.
															   // Therefore, vertex location is disregarded in light direction.

   halfVector = normalize(gl_LightSource[0].halfVector.xyz); // gl_LightSource[0].halfVector bisects the two vectors
                                                             // gl_LightSource[0].position and [0, 0, 1, 0] (= positive z-direction). 
															 // Therefore, it is correct to use this as the halfway vector between 
															 // light and eye only if the viewpoint is infinite, i.e., up the z-axis.
	
   // The following equations track word for word the first lighting equation given in Chapter 11.
   emission = gl_FrontMaterial.emission;   
   globalAmbient =  gl_LightModel.ambient * gl_FrontMaterial.ambient; 
   ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
   diffuse = max(dot(normal, lightDirection), 0.0) * (gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse); 
   specular = pow( max(dot(normal, halfVector), 0.0), gl_FrontMaterial.shininess ) * (gl_LightSource[0].specular * gl_FrontMaterial.specular);  
   gl_FrontColor =  emission + globalAmbient + ambient + diffuse + specular;

   gl_Position = ftransform();
}
