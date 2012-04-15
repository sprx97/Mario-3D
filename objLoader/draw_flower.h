#ifndef DRAW_FLOWER_H
#define DRAW_FLOWER_H
#include "draw_object.h"

class draw_flower : public draw_object {
 public: 
  draw_flower(glm::vec3,
			  glm::vec3,
			  glm::vec3);
  void load();
};
#endif
