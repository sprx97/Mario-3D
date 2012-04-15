#include "Mesh.h"
#include "draw_coin.h"
#include <string>

draw_coin::draw_coin(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				 : draw_object(p, s, r) {

	//set the parameters of body
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						  mPoint(scale.x, scale.y, scale.z),
						  mPoint(rot.x, rot.y, rot.z),
						  Material(mPoint(1,1,0), mPoint(1,1,0), mPoint(1,1,0))));

	hitboxes.push_back(new Cube(position.x, position.y, position.z, "brickblock", scale.x*15));

	load();
}

void draw_coin::setHitboxes() {
	hitboxes[0]->move(position.x, position.y, position.z);
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

//load all three
void draw_coin::load() {
  meshes[0].loadOBJ("../objLoader/coinfinal.obj");
}
