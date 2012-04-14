#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "../Cube/Cube.h"

using namespace std;

class draw_object {
	public:
		draw_object(float, float, float,
					float, float, float,
					float, float, float);
		
		float xpos, ypos, zpos;
		float xscale, yscale, zscale;
		float xrot, yrot, zrot;
		
		vector<Mesh> meshes;
		
		vector<Cube*> hitboxes;
		
		virtual void load() = 0; // pure virtual
		void draw();
		void collidesWith(Cube* c); // collision
};
#endif
