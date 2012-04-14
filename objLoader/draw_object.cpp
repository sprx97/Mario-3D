#include "draw_object.h"
#include <vector>
#include <iostream>

draw_object::draw_object(float xp, float yp, float zp,
						 float xs, float ys, float zs,
						 float xr, float yr, float zr) {
	xpos = xp; ypos = yp; zpos = zp;
	xscale = xs; yscale = ys; zscale = zs;
	xrot = xr; yrot = yr; zrot = zr;
}

void draw_object::draw() {
	if(meshes.size() == 0) {
		cout << "Zero meshes found. Object probably has not been initialized." << endl;
		return;
	}
	for(int n = 0; n < meshes.size(); n++) {
		meshes[n].draw();
	}
}
