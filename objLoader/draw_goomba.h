#ifndef DRAW_GOOMBA_H
#define DRAW_GOOMBA_H
#include "draw_object.h"

class draw_goomba : public draw_object {
 public: 
  draw_goomba(glm::vec3,
			  glm::vec3,
			  glm::vec3);
  void load();
};
#endif
