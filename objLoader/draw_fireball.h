#ifndef DRAW_FIREBALL_H
#define DRAW_FIREBALL_H
#include "draw_object.h"

class draw_fireball : public draw_object {
 public: 
  draw_fireball(float, float, float, 
				float, float, float, 
				float, float, float);
  void load();
};
#endif
