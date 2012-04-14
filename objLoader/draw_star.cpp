#include "Mesh.h"
#include "draw_star.h"
#include <string>

 draw_star::draw_star(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) 
				 : draw_object(xp, yp, zp, xs, ys, zs, xr, yr, zr) {

	//set the parameters of body
	meshes.push_back(Mesh(mPoint(xpos, ypos, zpos), 
						  mPoint(xscale, yscale, zscale),
						  mPoint(xrot, yrot, zrot),
						  Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,0,1))));
  
	//set parameters of eyes
	meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						  mPoint(xscale, yscale, zscale),
						  mPoint(xrot, yrot, zrot),
						  Material()));
	hitboxes.push_back(new Cube(xpos, ypos+6*yscale, zpos, "brickblock", 3*xscale));
}

//load all three
void draw_star::load() {
  meshes[0].loadOBJ("../objLoader/star_smooth.obj");
  meshes[1].loadOBJ("../objLoader/star_eyes.obj");
}
