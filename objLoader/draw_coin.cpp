#include "Mesh.h"
#include "draw_coin.h"
#include <string>

draw_coin::draw_coin(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) 
				 : draw_object(xp, yp, zp, xs, ys, zs, xr, yr, zr) {

	//set the parameters of body
	meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						  mPoint(xscale, yscale, zscale),
						  mPoint(xrot, yrot, zrot),
						  Material(mPoint(1,1,0), mPoint(1,1,0), mPoint(1,1,0))));
}

//load all three
void draw_coin::load() {
  meshes[0].loadOBJ("../objLoader/coinfinal.obj");
}
