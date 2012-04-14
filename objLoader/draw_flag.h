#ifndef DRAW_FLAG_H
#define DRAW_FLAG_H
#include "draw_object.h"

class draw_flag : public draw_object {
 public: 
	draw_flag(float, float, float, 
			  float, float, float, 
			  float, float, float);
	void load();
};
#endif
