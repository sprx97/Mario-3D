#ifndef _MESH_H_
#define _MESH_H_

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "Material.h"

using std::vector;
using std::string;
using std::ifstream;

// Mesh class
class Mesh {
	friend class PosableMesh;
	private:
		//variables that hold the state of our model
		vector<float> vertices;
		vector<float> vertexNormals;
		vector<float> vertexTexCoords;
		vector<int> vertexIndices;
		vector<int> vertexNormalIndices;
		vector<int> vertexTexCoordIndices;
		bool objHasVertexNormals;
		bool objHasVertexTexCoords;
		
		GLuint objTexHandle;
		
		// The display list that the mesh calls to draw itself
		GLuint objectDisplayList;

		// Material mesh is made of
		Material material;
		
		// Location and such of the mesh
		mPoint location;
		mPoint scale;
		mPoint rotation;
				

	public:
		// Constructors
		Mesh(const mPoint location = mPoint(),
			 const mPoint scale = mPoint(1,1,1),
			 const mPoint rotation = mPoint(),
			 const Material m = Material());
		Mesh(const Material m);
		
		// load from file, and draw functions
		void loadOBJ(const string &filename, bool makeDisplayList = true);
		void createOBJDisplayList();
		void draw();
		
		// Setters
		void setMaterial(const Material m);
		void setLocation(const mPoint p);
		void setScale(const mPoint p);
		void setRotation(const mPoint p);
		
		// Getters
		mPoint getLocation() const;
		mPoint getScale() const;
		mPoint getRotation() const;
		Material getMaterial() const;

};

#endif
