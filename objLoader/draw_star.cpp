#include "Mesh.h"
#include "draw_star.h"
#include <string>

 draw_star::draw_star(float xp, float yp, float zp,
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
  star.setLocation(mPoint(xpos, ypos, zpos));
  star.setScale(mPoint(xscale, yscale, zscale));
  star.setRotation(mPoint(xrot, yrot, zrot));
  star.setMaterial(Material(mPoint(1,1,0), mPoint(1,1,1), mPoint(1,0,1)));
  
  //set parameters of eyes
  eyes.setLocation(mPoint(xpos, ypos, zpos));
  eyes.setScale(mPoint(xscale, yscale, zscale));
  eyes.setRotation(mPoint(xrot, yrot, zrot));
  eyes.setMaterial(Material());
 
}


//load all three
void draw_star::load() {
  xrot++;
  star.setRotation(mPoint(xrot, yrot, zrot));
  star.loadOBJ("../objLoader/star_smooth.obj");
  eyes.loadOBJ("../objLoader/star_eyes.obj");

}
//draw all three in their respective colors/locations/textures
void draw_star::draw() {

  star.draw();
  eyes.draw();

}
