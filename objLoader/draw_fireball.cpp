#include "Mesh.h"
#include "draw_fireball.h"
#include <string>

 draw_fireball::draw_fireball(float xp, float yp, float zp,
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
  fireball.setLocation(mPoint(xpos, ypos, zpos));
  fireball.setScale(mPoint(xscale, yscale, zscale));
  fireball.setRotation(mPoint(xrot, yrot, zrot));
  fireball.setMaterial(Material(mPoint(1,.4,0), mPoint(1,.5,0), mPoint(1,1,0)));
   
}


//load all three
void draw_fireball::load() {
  xrot++;
  fireball.setRotation(mPoint(xrot, yrot, zrot));
  fireball.loadOBJ("../objLoader/fireball.obj");
 

}
//draw all three in their respective colors/locations/textures
void draw_fireball::draw() {

  fireball.draw();

}
