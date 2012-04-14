#ifndef DRAW_FLAG_H
#define DRAW_FLAG_H
#include "Mesh.h"

class draw_flag {


 public: 

  draw_flag(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();


  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;

  Mesh flag;
  Mesh flagpole;

};

#endif
