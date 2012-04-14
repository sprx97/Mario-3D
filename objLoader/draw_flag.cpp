#include "Mesh.h"
#include "draw_flag.h"
#include <string>

 draw_flag::draw_flag(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) 
				 : draw_object(xp, yp, zp, xs, ys, zs, xr, yr, zr) {
				 
  //set the parameters of body
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
				   mPoint(xscale, yscale, zscale),
				   mPoint(xrot, yrot, zrot),
				   Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1))));
  
  //set parameters of eyes
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
					    mPoint(xscale, yscale, zscale),
					    mPoint(xrot, yrot, zrot),
						Material(mPoint(0, 1, 0), mPoint(1, 1, 0), mPoint(1, 1, 0))));
 
}

//load all three
void draw_flag::load() {
  meshes[0].loadOBJ("../objLoader/flag.obj");
  meshes[1].loadOBJ("../objLoader/flagpole.obj");
}
