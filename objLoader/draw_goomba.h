#include "Mesh.h"
#ifndef DRAW_GOOMBA_H
#define DRAW_GOOMBA_H

class draw_goomba {


 public: 

  draw_goomba(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();


  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;



  Mesh white;
  Mesh black;
  Mesh head;
  Mesh body;
  Mesh feet;

};

#endif
