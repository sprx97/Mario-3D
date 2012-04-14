#include "Mesh.h"
#ifndef DRAW_PIPE_H
#define DRAW_PIPE_H

class draw_pipe {


 public: 

  draw_pipe(float, float, float, 
		float, float, float, 
		float, float, float);

  
  void load();
  void draw();


  float xpos, ypos, zpos;
  float xscale, yscale, zscale;
  float xrot, yrot, zrot;

  Mesh pipe;
  
};

#endif
