#ifndef DRAW_FLOWER_H
#define DRAW_FLOWER_H
#include "Mesh.h"

class draw_flower {


 public: 

  draw_flower(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();
  void setLocation(float, float, float);
  void setScale(float, float, float);
  void setRotation(float, float, float);

  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;

  Mesh stem_leaves;
  Mesh center;
  Mesh inner;
  Mesh outer;
};

#endif
