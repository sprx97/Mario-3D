#include "Mesh.h"
#include "draw_goomba.h"
#include <string>

 draw_goomba::draw_goomba(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) 
				 : draw_object(xp, yp, zp, xs, ys, zs, xr, yr, zr) {

  //set the parameters of white
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1))));

  // black
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material()));
 
  // head
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material(mPoint(.4,.13,.01), mPoint(.2,.1,0), mPoint(.2,.1,0))));

  // body
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material(mPoint(.8,.7,.5), mPoint(.8,.5,.4), mPoint(.8,.5, .4))));

  //feet of goomba
  meshes.push_back(Mesh(mPoint(xpos, ypos, zpos),
						mPoint(xscale, yscale, zscale),
						mPoint(xrot, yrot, zrot),
						Material(mPoint(.09,.05, .05), mPoint(.1,.1,.1), mPoint(0,0,0))));
}

//load all three
void draw_goomba::load() {
  meshes[0].loadOBJ("../objLoader/goombawhite.obj");
  meshes[1].loadOBJ("../objLoader/goombablack.obj");
  meshes[2].loadOBJ("../objLoader/goombahead.obj");
  meshes[3].loadOBJ("../objLoader/goombabody.obj");
  meshes[4].loadOBJ("../objLoader/goombafeet.obj");
}
