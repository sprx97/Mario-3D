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
		
		bool collidesWith(Cube*); // collision
		bool collidesX(Cube*);
		bool collidesY(Cube*);
		bool collidesZ(Cube*);
		bool collidesBottomY(Cube*);
		bool collidesTopY(Cube*);

		bool collidesWith(draw_object*); // collision
		bool collidesX(draw_object*);
		bool collidesY(draw_object*);
		bool collidesZ(draw_object*);
		bool collidesBottomY(draw_object*);
		bool collidesTopY(draw_object*);
		
		bool destroyed;

		glm::vec3 position;
};
#endif
