#include "draw_object.h"
#include <vector>
#include <iostream>

draw_object::draw_object(glm::vec3 p, glm::vec3 s, glm::vec3 r) {
	position = p;
	scale = s;
	rot = r;
	velocity = glm::vec3(0.0, 0.0, 0.0);
	destroyed = false;
}

void draw_object::setHitboxes() {
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->position = position;
		hitboxes[n]->velocity = velocity;
	}
}

void draw_object::draw() {
	setHitboxes();
	if(meshes.size() == 0) {
		cout << "Zero meshes found. Object probably has not been initialized." << endl;
		return;
	}
	for(int n = 0; n < meshes.size(); n++) {
		meshes[n].draw();
	}
}

bool draw_object::collidesWith(Cube* c) {
	setHitboxes();
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesWith(hitboxes[n]) || hitboxes[n]->collidesWith(c)) return true;
	}
	return false;
}

bool draw_object::collidesX(Cube* c) {
	setHitboxes();
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesX(hitboxes[n]) || hitboxes[n]->collidesX(c)) return true;
	}
	return false;
}

bool draw_object::collidesY(Cube* c) {
	setHitboxes();
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesY(hitboxes[n]) || hitboxes[n]->collidesY(c)) return true;
	}
	return false;
}

bool draw_object::collidesZ(Cube* c) {
	setHitboxes();
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesZ(hitboxes[n]) || hitboxes[n]->collidesZ(c)) return true;
	}
	return false;
}

bool draw_object::collidesBottomY(Cube* c) {
	setHitboxes();
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesBottomY(hitboxes[n]) || hitboxes[n]->collidesBottomY(c)) return true;
	}
	return false;
}

bool draw_object::collidesTopY(Cube* c) {
	setHitboxes();
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesTopY(hitboxes[n]) || hitboxes[n]->collidesTopY(c)) return true;
	}
	return false;
}
