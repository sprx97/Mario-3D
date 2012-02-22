// Jeremy Vercillo
// 2/15/12
// Final Project
// A class to store an OpenGL cube

#ifndef CUBE_H
#define CUBE_H

class Cube {
	public:
		float xpos, ypos, zpos, size;
		
		GLfloat vertices[72]; // vertecies of each face
		GLuint vbo_vertices; // VBO of coordinates
		
		GLfloat texcoords[48]; // coordinates of texture img to use
		GLuint vbo_texcoords; // VBO of texcoords
		
		GLushort elements[36]; // array indices of elements
		GLuint ibo_elements; // IBO of elements
		
		GLuint texture_id; // id code of the texture
		GLint uniform_mvp, uniform_mytexture;
		GLint attribute_coord3d, attribute_texcoord;

		const char* texturename;

		Cube(float, float, float, const char*, float);
		bool collidesWith(float, float, float);
		bool collidesWith(Cube*);
		void move(float, float, float);
		~Cube();
};
#endif
