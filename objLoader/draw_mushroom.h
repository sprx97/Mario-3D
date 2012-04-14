#ifndef DRAW_MUSHROOM_H
#define DRAW_MUSHROOM_H
#include "draw_object.h"

class draw_mushroom : public draw_object {
 public: 
  draw_mushroom(float, float, float, 
				float, float, float, 
				float, float, float);
  void load();
};
#endif
