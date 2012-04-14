#include "Mesh.h"
#include "draw_flower.h"
#include <string>
/*add get location function*/
 draw_flower::draw_flower(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) 
				 : draw_object(xp, yp, zp, xs, ys, zs, xr, yr, zr) {

	meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						  mPoint(xscale, yscale, zscale),
					      mPoint(xrot, yrot, zrot),
						  Material(mPoint(.1,1,0), mPoint(.1,1,0), mPoint(1,1,0))));
	meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						  mPoint(xscale, yscale, zscale),
					      mPoint(xrot, yrot, zrot),
						  Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1))));
	meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						  mPoint(xscale, yscale, zscale),
					      mPoint(xrot, yrot, zrot),
						  Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,1,0))));
	meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						  mPoint(xscale, yscale, zscale),
					      mPoint(xrot, yrot, zrot),
						  Material(mPoint(1,0,0), mPoint(1,1,1), mPoint(1,0,0))));

	hitboxes.push_back(new Cube(xpos, ypos, zpos, "brickblock", xscale*4));
}

//load all three
void draw_flower::load() {
  meshes[0].loadOBJ("../objLoader/stem_leaves.obj");
  meshes[1].loadOBJ("../objLoader/flower_center.obj");
  meshes[2].loadOBJ("../objLoader/flower_inner.obj");
  meshes[3].loadOBJ("../objLoader/flower_outer.obj");
}
