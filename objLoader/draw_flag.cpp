#include "Mesh.h"
#include "draw_flag.h"
#include <string>

 draw_flag::draw_flag(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				 : draw_object(p, s, r) {
				 
  //set the parameters of body
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
				   mPoint(scale.x, scale.y, scale.z),
				   mPoint(rot.x, rot.y, rot.z),
				   Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1))));
  
  //set parameters of eyes
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
					    mPoint(scale.x, scale.y, scale.z),
					    mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(0, 1, 0), mPoint(1, 1, 0), mPoint(1, 1, 0))));
  for(int n = -9; n < 10; n++) {
	hitboxes.push_back(new Cube(position.x, position.y+scale.y*n, position.z, "brickblock", scale.x));
  }

  type = "flag";

  load();
}

void draw_flag::setHitboxes() {
	for(int n = -9; n < 10; n++) {
		hitboxes[n+9]->move(position.x, position.y+scale.y*n, position.z);
	}
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

//load all three
void draw_flag::load() {
  meshes[0].loadOBJ("../objLoader/flag.obj");
  meshes[1].loadOBJ("../objLoader/flagpole.obj");
}
