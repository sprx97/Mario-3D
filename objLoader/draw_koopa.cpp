#include "Mesh.h"
#include "draw_koopa.h"
#include <string>

draw_koopa::draw_koopa(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				 : draw_enemy(p, s, r) {

	shell = new draw_shell(p, s, r);

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
 
	// bod
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(1, .6 ,0), mPoint(.2,.5,0), mPoint(.2, 1, 0))));

	//shell
	meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(.1, .6, .1), mPoint(.2,.5,0), mPoint(.2, .5, 0))));
	
	hitboxes.push_back(new Cube(position.x, position.y, position.z, "brickblock", .5*scale.x));

	type = "koopa";

	load();
}

void draw_koopa::setHitboxes() {
	hitboxes[0]->move(position.x, position.y, position.z);
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

//load all three
void draw_koopa::load() {
  meshes[0].loadOBJ("objLoader/turtleW.obj");
  meshes[1].loadOBJ("objLoader/turtleB.obj");
  meshes[2].loadOBJ("objLoader/turtlebod.obj");
  meshes[3].loadOBJ("objLoader/turtleG.obj");
    
}
