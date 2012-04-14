#ifndef DRAW_STAR_H
#define DRAW_STAR_H
#include "draw_object.h"

class draw_star : public draw_object {
 public: 
  draw_star(float, float, float, 
			float, float, float, 
			float, float, float);
  void load();
};
#endif
