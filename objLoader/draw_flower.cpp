#include "Mesh.h"
#include "draw_flower.h"
#include <string>
/*add get location function*/
 draw_flower::draw_flower(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				 : draw_object(p, s, r) {

	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						  mPoint(scale.x, scale.y, scale.z),
					      mPoint(rot.x, rot.y, rot.z),
						  Material(mPoint(.1,1,0), mPoint(.1,1,0), mPoint(1,1,0))));
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						  mPoint(scale.x, scale.y, scale.z),
					      mPoint(rot.x, rot.y, rot.z),
						  Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1))));
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						  mPoint(scale.x, scale.y, scale.z),
					      mPoint(rot.x, rot.y, rot.z),
						  Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,1,0))));
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						  mPoint(scale.x, scale.y, scale.z),
					      mPoint(rot.x, rot.y, rot.z),
						  Material(mPoint(1,0,0), mPoint(1,1,1), mPoint(1,0,0))));

	hitboxes.push_back(new Cube(position.x, position.y, position.z, "brickblock", scale.x*4));

    type = "flower";

	load();
}

void draw_flower::setHitboxes() {
	hitboxes[0]->move(position.x, position.y, position.z);
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

//load all three
void draw_flower::load() {
  meshes[0].loadOBJ("objLoader/stem_leaves.obj");
  meshes[1].loadOBJ("objLoader/flower_center.obj");
  meshes[2].loadOBJ("objLoader/flower_inner.obj");
  meshes[3].loadOBJ("objLoader/flower_outer.obj");
}
