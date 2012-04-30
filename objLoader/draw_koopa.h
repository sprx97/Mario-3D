#ifndef DRAW_KOOPA_H
#define DRAW_KOOPA_H
#include "draw_enemy.h"

class draw_koopa : public draw_enemy {
 public: 
  draw_koopa(glm::vec3,
			  glm::vec3,
			  glm::vec3);
  void load();
  void setHitboxes();
};
#endif
