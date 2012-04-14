#include "Mesh.h"
#include "draw_fireball.h"
#include <string>

 draw_fireball::draw_fireball(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) 
				 : draw_object(xp, yp, zp, xs, ys, zs, xr, yr, zr) {

  //set the parameters of body
	meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						  mPoint(xscale, yscale, zscale),
						  mPoint(xrot, yrot, zrot),
						  Material(mPoint(1,.4,0), mPoint(1,.5,0), mPoint(1,1,0))));

	hitboxes.push_back(new Cube(xpos, ypos, zpos, "brickblock", xscale));
}

//load all three
void draw_fireball::load() {
  meshes[0].loadOBJ("../objLoader/fireball.obj");
}
