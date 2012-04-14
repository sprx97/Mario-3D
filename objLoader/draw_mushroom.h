#ifndef DRAW_MUSHROOM_H
#define DRAW_MUSHROOM_H
#include "Mesh.h"

class draw_mushroom {


 public: 

  draw_mushroom(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();


  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;

  Mesh body;
  Mesh top;
  Mesh eyes;

};

#endif
