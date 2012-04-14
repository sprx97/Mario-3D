#ifndef DRAW_FLOWER_H
#define DRAW_FLOWER_H
#include "Mesh.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Cube/Cube.h"

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
  
  vector<Cube*> hitboxes;
};

#endif
