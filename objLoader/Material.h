/******************************************************
** CSE 40613 Computer Graphics
** Homework 06 -- Shader Programming
** Material.h
** Josh Wise
** 22 November 2011
*******************************************************/

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include <string>
#include <vector>
#include "mpoint.h"
#include "helper.h"

typedef mPoint Color; // I was gonna use this more, but didn't get around to it
using std::string;

// Material class
class Material {
	private:
		// Necessary values for Material
		Color diffuseReflectance;
		Color specularReflectance;
		Color ambientReflectance;
		float shininess;
		
		string textureFilename;
		bool isTextured;
	public:
		// Contructor
		Material(const mPoint d = mPoint(),
				 const mPoint sp = mPoint(),
				 const mPoint a = mPoint(),
				 const float sh = 64.0f);
		Material(const string &filename);
		// Set as material for object
		void setAsCurrentMaterial();
		
		// Setters
		void setDiffuseReflectance(const mPoint p);
		void setDiffuseReflectance(const float r, const float g, const float b);
		void setSpecularReflectance(const mPoint p);
		void setSpecularReflectance(const float r, const float g, const float b);
		void setAmbientReflectance(const mPoint p);
		void setAmbientReflectance(const float r, const float g, const float b);
		void setShininess(const float sh);
		
		// Getters
		mPoint getDiffuseReflectance() const;
		mPoint getSpecularReflectance() const;
		mPoint getAmbientReflectance() const;
		float getShininess() const;
		string getFilename() const;
		bool hasTextureFile() const;
};

#endif
