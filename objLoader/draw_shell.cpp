#include "Mesh.h"
#include "draw_shell.h"
#include <string>

draw_shell::draw_shell(glm::vec3 p, glm::vec3 s, glm::vec3 r)
		: draw_object(p, s, r){


 //set the parameters of white
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1))));

  //shell
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(.1, .6, .1), mPoint(.2,.5,0), mPoint(.2, .5, 0))));
  
  hitboxes.push_back(new Cube(position.x, position.y+.25*scale.y, position.z, "brickblock", .25*scale.x));
  
  type = "shell";
  
  load();
}

void draw_shell::setHitboxes() {
	hitboxes[0]->move(position.x, position.y+.25*scale.y, position.z);
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}


//load all three
void draw_shell::load() {
  meshes[0].loadOBJ("objLoader/shellWhite.obj");
  meshes[1].loadOBJ("objLoader/shellGreen.obj");
}
