#ifndef DRAW_FLAG_H
#define DRAW_FLAG_H
#include "draw_object.h"

class draw_flag : public draw_object {
 public: 
	draw_flag(glm::vec3,
			  glm::vec3,
			  glm::vec3);
	void load();
	void setHitboxes();
};
#endif
