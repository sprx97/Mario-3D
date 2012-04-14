#include "Mesh.h"
#include "draw_mushroom.h"
#include <string>

 draw_mushroom::draw_mushroom(float xp, float yp, float zp,
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
  body.setLocation(mPoint(xpos, ypos, zpos));
  body.setScale(mPoint(xscale, yscale, zscale));
  body.setRotation(mPoint(xrot, yrot, zrot));
  body.setMaterial(Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,0,1)));
  //set parameters of top
  //this should be polka dot texture, but isnt working
  top.setLocation(mPoint(xpos, ypos, zpos));
  top.setScale(mPoint(xscale, yscale, zscale));
  top.setRotation(mPoint(xrot, yrot, zrot));
  top.setMaterial(Material(mPoint(1,0,0), mPoint(1,1,1), mPoint(1,0,0)));
  //set parameters of eyes
  eyes.setLocation(mPoint(xpos, ypos, zpos));
  eyes.setScale(mPoint(xscale, yscale, zscale));
  eyes.setRotation(mPoint(xrot, yrot, zrot));
  eyes.setMaterial(Material());
 
}


//load all three
void draw_mushroom::load() {

  body.loadOBJ("../objLoader/mushroom_body.obj");
  top.loadOBJ("../objLoader/mushroom_top.obj");
  eyes.loadOBJ("../objLoader/mushroom_eyes.obj");

}
//draw all three in their respective colors/locations/textures
void draw_mushroom::draw() {

  body.draw();
  top.draw();
  eyes.draw();

}
