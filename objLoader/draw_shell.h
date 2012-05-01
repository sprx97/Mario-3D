#ifndef DRAW_SHELL_H
#define DRAW_SHELL_H
#include "draw_object.h"

class draw_shell : public draw_object {


 public: 

  draw_shell(glm::vec3, glm::vec3, glm::vec3);

  
  void load();
  void setHitboxes();

  

};

#endif
