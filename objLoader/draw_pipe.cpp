#include "Mesh.h"
#include "draw_pipe.h"
#include <string>

draw_pipe::draw_pipe(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr)
				: draw_object(xp, yp, zp, xs, ys, zs, xr, yr, zr) {
				
  //set the parameters of body
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material(mPoint(0,1,0), mPoint(1,1,0), mPoint(1,1,0))));

  hitboxes.push_back(new Cube(xpos, ypos+1.0*20*yscale, zpos, "brickblock", 20*xscale));
  hitboxes.push_back(new Cube(xpos, ypos+0.5*20*yscale, zpos, "brickblock", 20*xscale));
}

//load all three
void draw_pipe::load() {
  meshes[0].loadOBJ("../objLoader/pipesmooth.obj");
}
