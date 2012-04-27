#include "Mesh.h"
#include "draw_pipe.h"
#include <string>

draw_pipe::draw_pipe(glm::vec3 p, glm::vec3 s, glm::vec3 r, draw_pipe* l)
				: draw_object(p, s, r) {
				
  //set the parameters of body
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(0,1,0), mPoint(1,1,0), mPoint(1,1,0))));

  hitboxes.push_back(new Cube(position.x, position.y+1.0*20*scale.y, position.z, "brickblock", 20*scale.x));
  hitboxes.push_back(new Cube(position.x, position.y+0.5*20*scale.y, position.z, "brickblock", 20*scale.x));

  linkedpipe = l;

  type = "pipe";

  load();
}

void draw_pipe::setHitboxes() {
	hitboxes[0]->move(position.x, position.y+1.0*20*scale.y, position.z);
	hitboxes[1]->move(position.x, position.y+0.5*20*scale.y, position.z);
	for(int n = 0; n < hitboxes.size(); n++) {
		hitboxes[n]->velocity = velocity;
	}
}

//load all three
void draw_pipe::load() {
  meshes[0].loadOBJ("../objLoader/pipesmooth.obj");
}
