#include "Mesh.h"
#include "draw_coin.h"
#include <string>

 draw_coin::draw_coin(float xp, float yp, float zp,
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
  coin.setLocation(mPoint(xpos, ypos, zpos));
  coin.setScale(mPoint(xscale, yscale, zscale));
  coin.setRotation(mPoint(xrot, yrot, zrot));
  coin.setMaterial(Material(mPoint(1,1,0), mPoint(1,1,0), mPoint(1,1,0)));
   
}


//load all three
void draw_coin::load() {
  xrot++;
  coin.setRotation(mPoint(xrot, yrot, zrot));
  coin.loadOBJ("../objLoader/coinfinal.obj");
 

}
//draw all three in their respective colors/locations/textures
void draw_coin::draw() {

  coin.draw();

}
