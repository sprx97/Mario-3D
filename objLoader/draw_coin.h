#include "Mesh.h"
#ifndef DRAW_COIN_H
#define DRAW_COIN_H

class draw_coin {


 public: 

  draw_coin(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();


  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;

  Mesh coin;
  
};

#endif
