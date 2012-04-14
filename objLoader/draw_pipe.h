#ifndef DRAW_PIPE_H
#define DRAW_PIPE_H
#include "draw_object.h"

class draw_pipe : public draw_object {
 public: 
  draw_pipe(float, float, float, 
			float, float, float, 
			float, float, float);
  void load();
};
#endif
