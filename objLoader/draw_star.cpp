#include "Mesh.h"
#include "draw_star.h"
#include <string>

 draw_star::draw_star(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				 : draw_object(p, s, r) {

	//set the parameters of body
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z), 
						  mPoint(scale.x, scale.y, scale.z),
						  mPoint(rot.x, rot.y, rot.z),
						  Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,0,1))));
  
	//set parameters of eyes
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						  mPoint(scale.x, scale.y, scale.z),
						  mPoint(rot.x, rot.y, rot.z),
						  Material()));
	hitboxes.push_back(new Cube(position.x, position.y+6*scale.y, position.z, "brickblock", 3*scale.x));

	load();
}

void draw_star::setHitboxes() {
	hitboxes[0]->move(position.x, position.y+6*scale.y, position.z);
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

//load all three
void draw_star::load() {
  meshes[0].loadOBJ("../objLoader/star_smooth.obj");
  meshes[1].loadOBJ("../objLoader/star_eyes.obj");
}
