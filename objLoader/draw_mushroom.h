#ifndef DRAW_MUSHROOM_H
#define DRAW_MUSHROOM_H
#include "draw_object.h"

class draw_mushroom : public draw_object {
 public: 
  draw_mushroom(glm::vec3,
				glm::vec3,
				glm::vec3,
				int = 0);
  void load();
  void setHitboxes();
  
  int is1up;
};
#endif
