#include "colors.inc"    // The include files contain
#include "textures.inc"  // pre-defined scene elements
#include "glass.inc"
#include "metals.inc"
#include "skies.inc"
#include "shapes.inc"
#include "bravais.inc"
#include "arrows.inc"
#include "math.inc"

// les vecteurs du cristal
#declare a = <1,0,0>;
#declare b = <.31,.95,0>;
#declare c = <.4,.5,0.76>;

#declare camera_location = 5 * (x+y+.5*z)+ 3*z + y;
#declare camera_look_at = <0,0,0>+3*z + y;

//#declare camera_location = a/2+b/1.5-2*z;
//#declare camera_look_at = a/2+b/1.5;

global_settings{ max_trace_level 255 }

background { color DarkGreen }

// all setup
camera {
  orthographic
  location camera_location
  look_at camera_look_at
}


light_source {
	<10, 0, 0> 
	color White
        parallel
        point_at <0, 0, 0>
        shadowless
}
light_source {
	<0, 10, 0> 
	color White
        parallel
        point_at <0, 0, 0>
        shadowless
}
light_source {
	<0, 0, 10> 
	color White
        parallel
        point_at <0, 0, 0>
}

union{
  // the planes
  plane { <1,0,0>, -4 pigment {color LightWood} }
  //plane { <0,1,0>, 0 pigment {color LightWood*.9 transmit .3} }
  //plane { <0,0,1>, 0 pigment {color LightWood*.8 transmit .3}}
}

// Les trois directions principales
cylinder{<0,0,-10>,<0,0,10>, .01 pigment {color Grey}}
cylinder{<0,-10,0>,<0,10,0>, .01 pigment {color Grey}}
cylinder{<-10,0,0>,<10,0,0>, .01 pigment {color Grey}}

// La construction d'Ewald
#macro ewald(ki, gamma, delta, Alpha, phi)
  #local kf=vrotate(vrotate(ki, delta*x), -gamma*y);
  #local Q=kf-ki;
  #local n = vrotate(vrotate(vlength(Q)*x, Alpha*y), phi*z);
  #local xp = vnormalize(Q);
  #local zp = vcross(xp, z);
  #local yp = vcross(xp, zp);

  light_source {
          Q
          color White
          parallel
          point_at <0, 0, 0>
  }
  
  // Vecteurs incident et diffract�
  union{
    arrow(ki, Blue)
    arrow(kf, Green)
    translate -ki
    no_shadow
  }
  
  // Vecteur de diffraction
  arrow(Q, Red)
  
  // angle vartheta
  //object{secteur(x, Q.x*x+Q.y*y, Magenta, .5)}
  
  // angle tau
  //object{secteur(Q, n, ForestGreen, .5) no_shadow}

  // angle psi
  //arrow(VProject_Plane(n, Q), Yellow)
  sector(yp, VProject_Plane(n, Q), ForestGreen, .5)
  //sector(yp, -yp-0.0001*x, ForestGreen, .5)
  //object{ arrow_circular(Q, .5, .05, 0, 90, 1, Red) translate 1.1*Q no_shadow}

  arrow(n, Yellow)
  disc {0, vcross(ki, Q), vlength(Q) pigment{color Red+Blue transmit .1}}
  disc {0, Q, vlength(Q) pigment{color Red transmit .3}}
  //plane { <0,0,-1>, 0 pigment {color LightWood*.2 transmit .7} }
 
  // rep�re x', y', z' permettant de d�finir l'origine des psi
  //arrow(xp*vlength(Q), Grey)
  arrow(yp*vlength(Q), Grey)
  //arrow(zp*vlength(Q), Grey)
  
  // sph�res d'ewald
  merge{
    //sphere{0, vlength(Q) pigment {color Blue transmit .93}}
    //sphere{-ki, vlength(ki) pigment {color Green transmit .93}}
    no_shadow
  }
#end

ewald(-3*z, -45, 40, -60, 30)

// Rep�re orthonorm� de la ligne cristal de soleil.
object{repere_soleil translate 7.5*z no_shadow}
