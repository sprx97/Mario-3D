#include "Mesh.h"
#include "draw_pipe.h"
#include <string>

 draw_pipe::draw_pipe(float xp, float yp, float zp,
			     float xs, float ys, float zs,
			     float xr, float yr, float zr) {
   
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
  pipe.setLocation(mPoint(xpos, ypos, zpos));
  pipe.setScale(mPoint(xscale, yscale, zscale));
  pipe.setRotation(mPoint(xrot, yrot, zrot));
  pipe.setMaterial(Material(mPoint(0,1,0), mPoint(1,1,0), mPoint(1,1,0)));
   
}


//load all three
void draw_pipe::load() {
  xrot++;
  pipe.setRotation(mPoint(xrot, yrot, zrot));
  pipe.loadOBJ("../objLoader/pipesmooth.obj");
 

}
//draw all three in their respective colors/locations/textures
void draw_pipe::draw() {

  pipe.draw();

}
