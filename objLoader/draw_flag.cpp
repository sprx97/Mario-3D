#include "Mesh.h"
#include "draw_flag.h"
#include <string>

 draw_flag::draw_flag(float xp, float yp, float zp,
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
  flag.setLocation(mPoint(xpos, ypos, zpos));
  flag.setScale(mPoint(xscale, yscale, zscale));
  flag.setRotation(mPoint(xrot, yrot, zrot));
  flag.setMaterial(Material(mPoint(1,1,1), mPoint(1,1,1), mPoint(1,1,1)));
  
  //set parameters of eyes
  flagpole.setLocation(mPoint(xpos, ypos, zpos));
  flagpole.setScale(mPoint(xscale, yscale, zscale));
  flagpole.setRotation(mPoint(xrot, yrot, zrot));
  flagpole.setMaterial(Material(mPoint(0, 1, 0), mPoint(1, 1, 0), mPoint(1, 1, 0)));
 
}


//load all three
void draw_flag::load() {
  xrot++;
  flag.setRotation(mPoint(xrot, yrot, zrot));
  flag.loadOBJ("../objLoader/flag.obj");
  flagpole.loadOBJ("../objLoader/flagpole.obj");

}
//draw all three in their respective colors/locations/textures
void draw_flag::draw() {

  flag.draw();
  flagpole.draw();

}
