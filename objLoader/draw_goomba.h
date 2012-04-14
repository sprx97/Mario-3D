#ifndef DRAW_GOOMBA_H
#define DRAW_GOOMBA_H
#include "draw_object.h"

class draw_goomba : public draw_object {
 public: 
  draw_goomba(float, float, float, 
			  float, float, float, 
			  float, float, float);
  void load();
};
#endif
