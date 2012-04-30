// Jeremy Vercillo
// 4/30/12

#include "Mesh.h"
#include "draw_enemy.h"
#include <string>

draw_enemy::draw_enemy(glm::vec3 p, glm::vec3 s, glm::vec3 r)
		   : draw_object(p, s, r) {
	destroycountdown = -1;
	knockbackcountdown = -1;	   
}
