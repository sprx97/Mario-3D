#include "Mesh.h"
#ifndef DRAW_FIREBALL_H
#define DRAW_FIREBALL_H

class draw_fireball {


 public: 

  draw_fireball(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();


  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;

  Mesh fireball;
  
};

#endif
