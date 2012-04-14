#include "Mesh.h"
#include "draw_mushroom.h"
#include <string>

 draw_mushroom::draw_mushroom(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr)
				: draw_object(xp, yp, zp, xs, ys, zs, xr, yr, zr) {
  
  //set the parameters of body
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,0,1))));

  //this should be polka dot texture, but isnt working
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material(mPoint(1,0,0), mPoint(1,1,1), mPoint(1,0,0))));

  //set parameters of eyes
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material())); 
  
  hitboxes.push_back(new Cube(xpos, ypos+2*yscale, zpos+zscale, "brickblock", 2*xscale));
}

//load all three
void draw_mushroom::load() {
  meshes[0].loadOBJ("../objLoader/mushroom_body.obj");
  meshes[1].loadOBJ("../objLoader/mushroom_top.obj");
  meshes[2].loadOBJ("../objLoader/mushroom_eyes.obj");
}
