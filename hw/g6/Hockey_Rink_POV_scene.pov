//wrap the file with the version
#local Temp_version = version;
#version 3.7;
//==================================================
//POV-Ray Main scene file
//==================================================
//This file has been created by PoseRay v3.13.28.615
//3D model to POV-Ray/Moray Converter.
//Author: FlyerX
//Email: flyerx_2000@yahoo.com
//Web: https://sites.google.com/site/poseray/
//==================================================
//Files needed to run the POV-Ray scene:
//Hockey_Rink_POV_main.ini (initialization file - open this to render)
//Hockey_Rink_POV_scene.pov (scene setup of cameras, lights and geometry)
//Hockey_Rink_POV_geom.inc (geometry)
//Hockey_Rink_POV_mat.inc (materials)
// 
//==================================================
//Model Statistics:
//Number of triangular faces..... 8692
//Number of vertices............. 4660
//Number of normals.............. 17384
//Number of UV coordinates....... 5944
//Number of lines................ 0
//Number of materials............ 6
//Number of groups/meshes........ 51
//Number of subdivision faces.... 0
//UV boundaries........ from u,v=(-0.00325,-0.036553)
//                        to u,v=(1.059694,1.019441)
//Bounding Box....... from x,y,z=(-131.748732,-0.551539,-297.789209)
//                      to x,y,z=(119.953605,201.933379,294.526994)
//                 size dx,dy,dz=(251.702337,202.484918,592.316203)
//                  center x,y,z=(-5.8975635,100.69092,-1.63110750000001)
//                       diagonal 674.679696452373
//Surface area................... 306739.673858824
//             Smallest face area 0.00989201457666758
//              Largest face area 1164.88192137796
//Memory allocated for geometry.. 1 MByte
// 
//==================================================
//IMPORTANT:
//This file was designed to run with the following command line options: 
// +W320 +H240 +FN +AM1 +A0.3 +r3 +Q9 +C -UA +MV3.7
//if you are not using an INI file copy and paste the text above to the command line box before rendering
 
#include "rad_def.inc"
 
global_settings {
  //This setting is for alpha transparency to work properly.
  //Increase by a small amount if transparent areas appear dark.
   max_trace_level 15
   adc_bailout 0.01
   assumed_gamma 1
 
   radiosity{Rad_Settings(Radiosity_Default, off, off)}
}
#include "Hockey_Rink_POV_geom.inc" //Geometry
#include "colors.inc" // testray colors
 
//CAMERA PoseRayCAMERA
camera {
        perspective
        up <0,1,0>
        right -x*image_width/image_height
        location <-3.5527136788005E-15,0,1349.35939290475>
        look_at <-3.5527136788005E-15,0,1348.35939290475>
        angle 32.9346087425027 // horizontal FOV angle
        rotate <-10,0,0> //look from slightly above glass behind net
        
        translate <0,-100,-500>
        //rotate <-19.2548348985563,0,0> //pitch
        //rotate <0,42.4291980668491,0> //yaw
        //translate <-5.8975635,100.69092,-1.63110750000001>
        }
 
//PoseRay default Light attached to the camera
light_source {
              <-3.5527136788005E-15,0,1349.35939290475> //light position
              color rgb <1,1,1>*1.6
              parallel
              point_at <-3.5527136788005E-15,0,0>
              rotate <0,0,31.8376824492703> //roll
              rotate <-19.2548348985563,0,0> //elevation
              rotate <0,42.4291980668491,0> //rotation
             }

light_source {
              <3.5527136788005,0,-103.35939290475> //light position
              color rgb <1,1,1>*1
              parallel
              point_at <-3.5527136788005E-15,10,50>
             }
 
//Background
background { color srgb <0,0,0>  }
 
//Assembled object that is contained in Hockey_Rink_POV_geom.inc with no SSLT components
object{
      Hockey_Rink_
      
      }

sor {
  4
  <1, 0>
  <1, 0.05>
  <1, 0.06>
  <1, 0.07>
  scale 30

  texture{
    pigment{
      image_map{
        png "B.png"
        map_type 1
        interpolate 2
      }
    //translate <-1,0,0>
    rotate <0,0,0>
    //scale 2
    }
  }
  finish{
        reflection 0.5
      }
}

//far crowd
box {
  <-200,-100000000,-300>, <1000,150,-300>
  texture {
    pigment {
      image_map {
        png "stands.png"
        map_type 0
      }
    }
    scale 200
  }
}

//left side crowd
box {
  <-190,-100000,400>, <-200,150,-300>
  texture {
    pigment {
      image_map {
        png "stands.png"
        map_type 0
      }
    }
    rotate <0,90,0>
    scale 200
  }
}

//right side crowd
box {
  <200,-100000,400>, <200,50,-300>
  texture {
    pigment {
      image_map {
        png "stands.png"
        map_type 0
      }
    }
    rotate <0,90,0>
    scale 200
  }
}

//ice
box {
  <-100,1,-200>, <100,1,400>
  texture {
    pigment {
      image_map {
        png "ice.png"
        map_type 0
        once
      }
    }
    rotate <0,90,0>
    scale 200
  }
  finish{
        reflection 0.5
      }
}


plane { y, -.9
  texture{
   pigment {
        image_map{
        //jpeg "snow.jpeg"
        jpeg "snow2.jpeg"
        //png "stands.png"
        map_type 0
        }
        scale <1,100,10>
      }
    }
   finish {ambient 0.1 diffuse 0.5}
}


//sphere{ <0,1,2>,90
//      texture{
//        pigment{
//          image_map{
//            png "B.png"
//            map_type 1
//            interpolate 2
//            once
//          }
//        }
//      }
//
//      rotate <0,0,0>
//      }

 
//restore the version used outside this file
#version Temp_version;
//==================================================
