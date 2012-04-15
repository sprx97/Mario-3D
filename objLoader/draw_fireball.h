#ifndef DRAW_FIREBALL_H
#define DRAW_FIREBALL_H
#include "draw_object.h"

class draw_fireball : public draw_object {
 public: 
  draw_fireball(glm::vec3,
			    glm::vec3,
				glm::vec3);
  void load();
  void setHitboxes();
};
#endif
