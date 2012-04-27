#ifndef DRAW_PIPE_H
#define DRAW_PIPE_H
#include "draw_object.h"

class draw_pipe : public draw_object {
 public: 
  draw_pipe(glm::vec3,
			glm::vec3,
			glm::vec3, draw_pipe* = NULL);
  void load();
  void setHitboxes();
  draw_pipe* linkedpipe;
};
#endif
