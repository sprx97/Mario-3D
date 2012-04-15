#ifndef DRAW_PIPE_H
#define DRAW_PIPE_H
#include "draw_object.h"

class draw_pipe : public draw_object {
 public: 
  draw_pipe(glm::vec3,
			glm::vec3,
			glm::vec3);
  void load();
};
#endif
