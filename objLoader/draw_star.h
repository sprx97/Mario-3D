#ifndef DRAW_STAR_H
#define DRAW_STAR_H
#include "draw_object.h"

class draw_star : public draw_object {
 public: 
  draw_star(glm::vec3, 
			glm::vec3, 
			glm::vec3);
  void load();
};
#endif
