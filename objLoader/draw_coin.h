#ifndef DRAW_COIN_H
#define DRAW_COIN_H
#include "draw_object.h"

class draw_coin : public draw_object {
 public: 
  draw_coin(glm::vec3,
			glm::vec3,
			glm::vec3);
  void load();
};
#endif
