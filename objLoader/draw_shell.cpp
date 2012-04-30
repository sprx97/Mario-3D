#include "Mesh.h"
#include "draw_shell.h"
#include <string>

draw_shell::draw_shell(glm::vec3 p, glm::vec3 s, glm::vec3 r)
			: draw_object(p, s, r) {
	
	// create meshes
	
	// create hitboxes
	
	type = "shell";
	
	load();
}

void draw_shell::setHitboxes() {
	// set hitbox correctly
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

void draw_shell::load() {
	// meshes load obj
}
