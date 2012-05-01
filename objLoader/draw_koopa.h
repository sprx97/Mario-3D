#ifndef DRAW_KOOPA_H
#define DRAW_KOOPA_H
#include "draw_enemy.h"
#include "draw_shell.h"

class draw_koopa : public draw_enemy {
 public: 
  draw_koopa(glm::vec3,
			  glm::vec3,
			  glm::vec3);
  void load();
  void setHitboxes();
  
  draw_shell* shell;
};
#endif
