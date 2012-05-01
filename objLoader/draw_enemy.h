#ifndef DRAW_ENEMY_H
#define DRAW_ENEMY_H
#include "draw_object.h"

class draw_enemy : public draw_object {
 public: 
  draw_enemy(glm::vec3,
			  glm::vec3,
			  glm::vec3);
  void load() = 0;
  void setHitboxes() = 0;
  
  int destroycountdown;
  int knockbackcountdown;
};
#endif
