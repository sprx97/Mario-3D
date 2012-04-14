#include "Mesh.h"
#include "draw_flower.h"
#include <string>
/*add get location function*/
 draw_flower::draw_flower(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) {
   //constructor initializes stuff!
  setLocation(xp, yp, zp);
  setScale(xs, ys, zs);
  setRotation(xr, yr, zr);

  stem_leaves.setMaterial(Material(mPoint(.1,1,0), mPoint(.1,1,0), mPoint(1,1,0)));
  center.setMaterial(Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1)));
  inner.setMaterial(Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,1,0)));
  outer.setMaterial(Material(mPoint(1,0,0), mPoint(1,1,1), mPoint(1,0,0)));
}

void draw_flower::setRotation(float x, float y, float z) {
	xrot = x; yrot = y; zrot = z;
	stem_leaves.setRotation(mPoint(xrot, yrot, zrot));
	center.setRotation(mPoint(xrot, yrot, zrot));
	inner.setRotation(mPoint(xrot, yrot, zrot));
	outer.setRotation(mPoint(xrot, yrot, zrot));
} // sets the new rotation for the flower

//void draw_flower::getLocation(){

void draw_flower::setLocation(float x, float y, float z) {
	xpos = x; ypos = y; zpos = z;
	stem_leaves.setLocation(mPoint(xpos, ypos, zpos));
	center.setLocation(mPoint(xpos, ypos, zpos));
	inner.setLocation(mPoint(xpos, ypos, zpos));
	outer.setLocation(mPoint(xpos, ypos, zpos));
} // sets the new location of the flower

void draw_flower::setScale(float x, float y, float z) {
	xscale = x; yscale = y; zscale = z;
	stem_leaves.setScale(mPoint(xscale, yscale, zscale));
	center.setScale(mPoint(xscale, yscale, zscale));
	inner.setScale(mPoint(xscale, yscale, zscale));
	outer.setScale(mPoint(xscale, yscale, zscale));
}

//load all three
void draw_flower::load() {
  stem_leaves.loadOBJ("../objLoader/stem_leaves.obj");
  center.loadOBJ("../objLoader/flower_center.obj");
  inner.loadOBJ("../objLoader/flower_inner.obj");
  outer.loadOBJ("../objLoader/flower_outer.obj");
}
//draw all three in their respective colors/locations/textures
void draw_flower::draw() {
  stem_leaves.draw();
  center.draw();
  inner.draw();
  outer.draw();
}
