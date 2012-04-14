#include "Mesh.h"
#include "draw_goomba.h"
#include <string>

 draw_goomba::draw_goomba(float xp, float yp, float zp,
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
  white.setLocation(mPoint(xpos, ypos, zpos));
  white.setScale(mPoint(xscale, yscale, zscale));
  
  white.setMaterial(Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1)));
  //set parameters of top
  //this should be polka dot texture, but isnt working
  
  black.setLocation(mPoint(xpos, ypos, zpos));
  black.setScale(mPoint(xscale, yscale, zscale));
  black.setMaterial(Material());
  //set parameters of eyes
  head.setLocation(mPoint(xpos, ypos, zpos));
  head.setScale(mPoint(xscale, yscale, zscale));
  head.setMaterial(Material(mPoint(.4,.13,.01), mPoint(.2,.1,0), mPoint(.2,.1,0)));
//set parameters of eyes
  body.setLocation(mPoint(xpos, ypos, zpos));
  body.setScale(mPoint(xscale, yscale, zscale));
  body.setMaterial(Material(mPoint(.8,.7,.5), mPoint(.8,.5,.4), mPoint(.8,.5, .4)));
  //feet of goomba
  feet.setLocation(mPoint(xpos, ypos, zpos));
  feet.setScale(mPoint(xscale, yscale, zscale));
  feet.setMaterial(Material(mPoint(.09,.05, .05), mPoint(.1,.1,.1), mPoint(0,0,0)));
  
}


//load all three
void draw_goomba::load() {

  white.loadOBJ("../objLoader/goombawhite.obj");
  black.loadOBJ("../objLoader/goombablack.obj");
  head.loadOBJ("../objLoader/goombahead.obj");
  body.loadOBJ("../objLoader/goombabody.obj");
  feet.loadOBJ("../objLoader/goombafeet.obj");
}
//draw all three in their respective colors/locations/textures
void draw_goomba::draw() {
  white.setRotation(mPoint(xrot, yrot, zrot));
  black.setRotation(mPoint(xrot, yrot, zrot));
  head.setRotation(mPoint(xrot, yrot, zrot));
  body.setRotation(mPoint(xrot, yrot, zrot));
  feet.setRotation(mPoint(xrot, yrot, zrot));
  white.draw();
  black.draw();
  head.draw();
  body.draw();
  feet.draw();

  //yrot++;
}

