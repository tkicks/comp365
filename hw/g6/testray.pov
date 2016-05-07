

#include "colors.inc"
#include "textures.inc"

// basic camera line up on Z axis, looking at origin
#declare camLoc = <0,0,-1.8>;

camera {
     location camLoc
     up y
     right x
     direction z  // look towards +z
}

#declare plainBall = sphere { <0,0,0>, 0.5 };

object { plainBall
   texture { White_Marble
	scale <0.3, 0.3, 0.3>
   }
 }

plane { y, -0.51
   pigment {DarkGreen}
   finish {ambient 0.1 diffuse 0.5}
}

background { color Blue }

light_source { <-30,40, -40> color White }