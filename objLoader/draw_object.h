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
		
		glm::vec3 velocity;
		glm::vec3 scale;
		glm::vec3 rot;
		
		const char* type;
		
		vector<Mesh> meshes;
		
		vector<Cube*> hitboxes;
		
		virtual void setHitboxes() = 0;
		virtual void load() = 0; // pure virtual
		void move(glm::vec3);
		void rotate(glm::vec3);
		void draw();
		
		bool intersectsWith(Cube*);
		
		bool collidesWith(Cube*, float); // collision
		bool collidesX(Cube*, float);
		bool collidesY(Cube*, float);
		bool collidesZ(Cube*, float);
		bool collidesBottomY(Cube*, float);
		bool collidesTopY(Cube*, float);

		bool collidesWith(draw_object*, float); // collision
		bool collidesX(draw_object*, float);
		bool collidesY(draw_object*, float);
		bool collidesZ(draw_object*, float);
		bool collidesBottomY(draw_object*, float);
		bool collidesTopY(draw_object*, float);
		
		bool destroyed;

		int destroycountdown;
		int knockbackcountdown;

		glm::vec3 position;
};
#endif
