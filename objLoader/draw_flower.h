#include "Mesh.h"
#ifndef DRAW_FLOWER_H
#define DRAW_FLOWER_H

class draw_flower {


 public: 

  draw_flower(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();


  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;

  Mesh stem_leaves;
  Mesh center;
  Mesh inner;
  Mesh outer;
};

#endif
