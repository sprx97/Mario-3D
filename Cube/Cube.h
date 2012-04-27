// Jeremy Vercillo
// 2/15/12
// Final Project
// A class to store an OpenGL cube

#ifndef CUBE_H
#define CUBE_H

class draw_object;

class Cube {
	public:
		glm::vec3 position, velocity;
		
		int prizetype;
	
//		GLfloat specrefon[];
//		GLfloat specrefoff[];
		float size;
		
		GLfloat vertices[72]; // vertecies of each face
		GLuint vbo_vertices; // VBO of coordinates
		
		GLfloat texcoords[48]; // coordinates of texture img to use
		GLuint vbo_texcoords; // VBO of texcoords
		
		GLushort elements[36]; // array indices of elements
		GLuint ibo_elements; // IBO of elements
		
		GLuint texture_id; // id code of the texture
    
       // void setReflect(int);
    
		const char* texturename;

		Cube(float, float, float, const char*, float, int = -1);
		bool intersectsWith(float, float, float);
		bool intersectsWith(Cube*);
		bool collidesWith(Cube*);
		bool collidesX(Cube*);
		bool collidesY(Cube*);
		bool collidesBottomY(Cube*);
		bool collidesTopY(Cube*);
		bool collidesZ(Cube*);
		void move(float, float, float);
		void draw(glm::mat4, glm::mat4, GLint, GLint, GLint);
		
		bool destroyed;
		bool hit;
		void printPos();
		~Cube();
};
#endif
