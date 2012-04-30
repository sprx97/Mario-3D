#ifndef DRAW_KOOPA_H
#define DRAW_KOOPA_H
#include "draw_object.h"

class draw_koopa : public draw_object {


 public: 

  draw_koopa(glm::vec3, glm::vec3, glm::vec3);

  
  void load();
  void setHitboxes();

  

};

#endif
