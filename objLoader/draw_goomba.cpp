#include "Mesh.h"
#include "draw_goomba.h"
#include <string>

 draw_goomba::draw_goomba(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				 : draw_enemy(p, s, r) {

  destroycountdown = -1;
  knockbackcountdown = -1;

  //set the parameters of white
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1))));

  // black
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material()));
 
  // head
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(.4,.13,.01), mPoint(.2,.1,0), mPoint(.2,.1,0))));

  // body
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(.8,.7,.5), mPoint(.8,.5,.4), mPoint(.8,.5, .4))));

  //feet of goomba
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(.09,.05, .05), mPoint(.1,.1,.1), mPoint(0,0,0))));

  hitboxes.push_back(new Cube(position.x, position.y+2*scale.y, position.z, "brickblock", 2*scale.x));

  type = "goomba";

  load();
}

void draw_goomba::setHitboxes() {
	hitboxes[0]->move(position.x, position.y+2*scale.y, position.z);
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

//load all three
void draw_goomba::load() {
  meshes[0].loadOBJ("../objLoader/goombawhite.obj");
  meshes[1].loadOBJ("../objLoader/goombablack.obj");
  meshes[2].loadOBJ("../objLoader/goombahead.obj");
  meshes[3].loadOBJ("../objLoader/goombabody.obj");
  meshes[4].loadOBJ("../objLoader/goombafeet.obj");
}
