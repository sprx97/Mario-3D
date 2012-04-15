#include "Mesh.h"
#include "draw_pipe.h"
#include <string>

draw_pipe::draw_pipe(glm::vec3 p, glm::vec3 s, glm::vec3 r)
				: draw_object(p, s, r) {
				
  //set the parameters of body
  meshes.push_back(Mesh(mPoint(position.x, position.y, position.z),
						mPoint(scale.x, scale.y, scale.z),
						mPoint(rot.x, rot.y, rot.z),
						Material(mPoint(0,1,0), mPoint(1,1,0), mPoint(1,1,0))));

  hitboxes.push_back(new Cube(position.x, position.y+1.0*20*scale.y, position.z, "brickblock", 20*scale.x));
  hitboxes.push_back(new Cube(position.x, position.y+0.5*20*scale.y, position.z, "brickblock", 20*scale.x));
}

//load all three
void draw_pipe::load() {
  meshes[0].loadOBJ("../objLoader/pipesmooth.obj");
}
