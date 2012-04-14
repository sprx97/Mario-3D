#ifndef DRAW_GOOMBA_H
#define DRAW_GOOMBA_H
#include "Mesh.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Cube/Cube.h"

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
  
  vector<Cube*> hitboxes;

};

#endif
