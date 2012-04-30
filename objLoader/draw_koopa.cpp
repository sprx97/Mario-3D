#include "Mesh.h"
#include "draw_koopa.h"
#include <string>

 draw_koopa::draw_koopa(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				 : draw_enemy(p, s, r) {

	destroycountdown = -1;
	knockbackcountdown = -1;

	// create meshes
	
	// create hitboxes

	type = "koopa";

	load();
}

void draw_koopa::setHitboxes() {
	// set hitbox correctly
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

//load all three
void draw_koopa::load() {
	// meshes load obj
}
