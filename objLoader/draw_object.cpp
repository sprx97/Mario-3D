#include "draw_object.h"
#include <vector>
#include <iostream>

draw_object::draw_object(glm::vec3 p, glm::vec3 s, glm::vec3 r) {
	position = p;
	scale = s;
	rot = r;
	velocity = glm::vec3(0.0, 0.0, 0.0);
	destroyed = false;
	type = "object";
	destroycountdown = -1;
	knockbackcountdown = -1;
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

void draw_object::move(glm::vec3 newpos) {
	position = newpos;
	for(int n = 0; n < meshes.size(); n++) {
		meshes[n].setLocation(mPoint(position.x, position.y, position.z));
	}
	setHitboxes();
}

void draw_object::rotate(glm::vec3 newrot) {
	rot = newrot;
	for(int n = 0; n < meshes.size(); n++) {
		meshes[n].setRotation(mPoint(rot.x,rot.y,rot.z));
	}
	setHitboxes();
}

bool draw_object::intersectsWith(Cube* c) {
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->intersectsWith(hitboxes[n])) return true;
	}
	return false;
}

bool draw_object::collidesWith(Cube* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesWith(hitboxes[n], dt) || hitboxes[n]->collidesWith(c, dt)) return true;
	}
	return false;
}

bool draw_object::collidesWith(draw_object* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		for(int m = 0; m < c->hitboxes.size(); m++) {
			if(hitboxes[n]->collidesWith(c->hitboxes[m], dt)) return true;
		}
	}
	return false;
}

bool draw_object::collidesX(Cube* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesX(hitboxes[n], dt) || hitboxes[n]->collidesX(c, dt)) return true;
	}
	return false;
}

bool draw_object::collidesX(draw_object* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		for(int m = 0; m < c->hitboxes.size(); m++) {
			if(hitboxes[n]->collidesX(c->hitboxes[m], dt)) return true;
		}
	}
	return false;
}

bool draw_object::collidesY(Cube* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesY(hitboxes[n], dt) || hitboxes[n]->collidesY(c, dt)) return true;
	}
	return false;
}

bool draw_object::collidesY(draw_object* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		for(int m = 0; m < c->hitboxes.size(); m++) {
			if(hitboxes[n]->collidesY(c->hitboxes[m], dt)) return true;
		}
	}
	return false;
}

bool draw_object::collidesZ(Cube* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		if(c->collidesZ(hitboxes[n], dt) || hitboxes[n]->collidesZ(c, dt)) return true;
	}
	return false;
}

bool draw_object::collidesZ(draw_object* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		for(int m = 0; m < c->hitboxes.size(); m++) {
			if(hitboxes[n]->collidesZ(c->hitboxes[m], dt)) return true;
		}
	}
	return false;
}

bool draw_object::collidesBottomY(Cube* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		if(hitboxes[n]->collidesBottomY(c, dt)) return true;
	}
	return false;
}

bool draw_object::collidesBottomY(draw_object* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		for(int m = 0; m < c->hitboxes.size(); m++) {
			if(hitboxes[n]->collidesBottomY(c->hitboxes[m], dt)) return true;
		}
	}
	return false;
}

bool draw_object::collidesTopY(Cube* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		if(hitboxes[n]->collidesTopY(c, dt)) return true;
	}
	return false;
}

bool draw_object::collidesTopY(draw_object* c, float dt) {
	for(int n = 0; n < hitboxes.size(); n++) {
		for(int m = 0; m < c->hitboxes.size(); m++) {
			if(hitboxes[n]->collidesTopY(c->hitboxes[m], dt)) return true;
		}
	}
	return false;
}
