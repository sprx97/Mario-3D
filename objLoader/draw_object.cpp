#include "draw_object.h"
#include <vector>
#include <iostream>

draw_object::draw_object(glm::vec3 p, glm::vec3 s, glm::vec3 r) {
	position = p;
	scale = s;
	rot = r;
	velocity = glm::vec3(0.0, 0.0, 0.0);
}

void draw_object::draw() {
	if(meshes.size() == 0) {
		cout << "Zero meshes found. Object probably has not been initialized." << endl;
		return;
	}
	for(int n = 0; n < meshes.size(); n++) {
		meshes[n].draw();
	}
}

bool draw_object::collidesWith(Cube* c) {
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesWith(hitboxes[n]) || hitboxes[n]->collidesWith(c)) return true;
	}
	return false;
}
