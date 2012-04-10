// Jeremy Vercillo
// 2/15/12
// Final Project
// A class to store an OpenGL cube

#ifndef CUBE_H
#define CUBE_H

class Cube {
	public:
		glm::vec3 position, velocity;	
	
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
		GLint uniform_mvp, uniform_mytexture;
		GLint attribute_coord3d, attribute_texcoord;
    
       // void setReflect(int);
    
		const char* texturename;

		Cube(float, float, float, const char*, float);
		bool intersectsWith(float, float, float);
		bool collidesWith(Cube*);
		bool collidesX(Cube*);
		bool collidesY(Cube*);
		bool collidesBottomY(Cube*);
		bool collidesTopY(Cube*);
		bool collidesZ(Cube*);
		void move(float, float, float);
		
		bool destroyed;
		bool hit;
		void printPos();
		~Cube();
};
#endif
