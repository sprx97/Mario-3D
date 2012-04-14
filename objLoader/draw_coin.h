#ifndef DRAW_COIN_H
#define DRAW_COIN_H
#include "draw_object.h"

class draw_coin : public draw_object {
 public: 
  draw_coin(float, float, float, 
			float, float, float, 
			float, float, float);
  void load();
};
#endif
