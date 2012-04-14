#ifndef DRAW_FIREBALL_H
#define DRAW_FIREBALL_H
#include "Mesh.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Cube/Cube.h"

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
  
  vector<Cube*> hitboxes;
};

#endif
