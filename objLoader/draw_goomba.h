#ifndef DRAW_GOOMBA_H
#define DRAW_GOOMBA_H
#include "draw_enemy.h"

class draw_goomba : public draw_enemy {
 public: 
  draw_goomba(glm::vec3,
			  glm::vec3,
			  glm::vec3);
  void load();
  void setHitboxes();
};
#endif
