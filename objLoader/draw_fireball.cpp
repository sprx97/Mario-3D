#include "Mesh.h"
#include "draw_fireball.h"
#include <string>

 draw_fireball::draw_fireball(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				 : draw_object(p, s, r) {

  //set the parameters of body
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						  mPoint(scale.x, scale.y, scale.z),
						  mPoint(rot.x, rot.y, rot.z),
						  Material(mPoint(1,.4,0), mPoint(1,.5,0), mPoint(1,1,0))));

	hitboxes.push_back(new Cube(position.x, position.y, position.z, "brickblock", scale.x));
}

//load all three
void draw_fireball::load() {
  meshes[0].loadOBJ("../objLoader/fireball.obj");
}
