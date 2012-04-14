#ifndef DRAW_FLOWER_H
#define DRAW_FLOWER_H
#include "draw_object.h"

class draw_flower : public draw_object {
 public: 
  draw_flower(float, float, float, 
			  float, float, float, 
			  float, float, float);
  void load();
};
#endif
