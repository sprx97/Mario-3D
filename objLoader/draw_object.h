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
		draw_object(glm::vec3,
					glm::vec3,
					glm::vec3);
		
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 scale;
		glm::vec3 rot;
		
		vector<Mesh> meshes;
		
		vector<Cube*> hitboxes;
		
		void setHitboxes();

		virtual void load() = 0; // pure virtual
		void draw();
		bool collidesWith(Cube*); // collision
		bool collidesX(Cube*);
		bool collidesY(Cube*);
		bool collidesZ(Cube*);
		bool collidesBottomY(Cube*);
		bool collidesTopY(Cube*);
		
		bool destroyed;
};
#endif
