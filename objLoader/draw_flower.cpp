#include "Mesh.h"
#include "draw_flower.h"
#include <string>

 draw_flower::draw_flower(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) {
   //constructor initializes stuff!
  xpos = xp;
  ypos = yp;
  zpos = zp;
  xscale = xs;
  yscale = ys;
  zscale = zs;
  xrot= xr;
  yrot = yr;
  zrot = zr;

  //set the parameters of body
  stem_leaves.setLocation(mPoint(xpos, ypos, zpos));
  stem_leaves.setScale(mPoint(xscale, yscale, zscale));
  
  stem_leaves.setMaterial(Material(mPoint(.1,1,0), mPoint(.1,1,0), mPoint(1,1,0)));
  //set parameters of top
  //this should be polka dot texture, but isnt working
  center.setLocation(mPoint(xpos, ypos, zpos));
  center.setScale(mPoint(xscale, yscale, zscale));
  
  center.setMaterial(Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1)));
  //set parameters of eyes
  inner.setLocation(mPoint(xpos, ypos, zpos));
  inner.setScale(mPoint(xscale, yscale, zscale));
  
  inner.setMaterial(Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,1,0)));
//set parameters of eyes
  outer.setLocation(mPoint(xpos, ypos, zpos));
  outer.setScale(mPoint(xscale, yscale, zscale));
  
  outer.setMaterial(Material(mPoint(1,0,0), mPoint(1,1,1), mPoint(1,0,0)));
 
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
  center.setRotation(mPoint(xrot, yrot, zrot));
  stem_leaves.setRotation(mPoint(xrot, yrot, zrot));
  inner.setRotation(mPoint(xrot, yrot, zrot));
  outer.setRotation(mPoint(xrot, yrot, zrot));
  stem_leaves.draw();
  center.draw();
  inner.draw();
  outer.draw();
  yrot++;
}

