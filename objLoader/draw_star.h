#ifndef DRAW_STAR_H
#define DRAW_STAR_H
#include "Mesh.h"

class draw_star {


 public: 

  draw_star(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();


  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;

  Mesh star;
  Mesh eyes;

};

#endif
