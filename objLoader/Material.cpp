/******************************************************
** CSE 40613 Computer Graphics
** Homework 06 -- Shader Programming
** Material.cpp
** Josh Wise
** 22 November 2011
*******************************************************/

#include "Material.h"

// Material constructor; has default values
Material::Material(const mPoint d,
				   const mPoint sp,
				   const mPoint a,
				   const float sh) : diffuseReflectance(d),
									 specularReflectance(sp),
									 ambientReflectance(a),
									 shininess(sh), isTextured(false) {}

Material::Material(const string &filename) : textureFilename(filename),
											 isTextured(true) {}

// Function to set as current material before rendering mesh
void Material::setAsCurrentMaterial() {
	float diffuse[4] = {diffuseReflectance.x,
						diffuseReflectance.y,
						diffuseReflectance.z, 1.0};
	float ambient[4] = {ambientReflectance.x,
						ambientReflectance.y,
						ambientReflectance.z, 1.0};                                                 
	float specular[4] = {specularReflectance.x,
						 specularReflectance.y,
						 specularReflectance.z, 1.0};

	//set these to be active
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

// GETTERS AND SETTERS
// pretty boring

void Material::setDiffuseReflectance(const mPoint p) {
	diffuseReflectance = p;
}

void Material::setSpecularReflectance(const mPoint p) {
	specularReflectance = p;
}

void Material::setAmbientReflectance(const mPoint p) {
	ambientReflectance = p;
}

void Material::setDiffuseReflectance(const float r, const float g, const float b) {
	setDiffuseReflectance(mPoint(r, g, b));
}

void Material::setSpecularReflectance(const float r, const float g, const float b) {
	setSpecularReflectance(mPoint(r, g, b));
}

void Material::setAmbientReflectance(const float r, const float g, const float b) {
	setAmbientReflectance(mPoint(r, g, b));
}

void Material::setShininess(const float sh) {
	shininess = sh;
}

mPoint Material::getDiffuseReflectance() const {
	return diffuseReflectance;
}

mPoint Material::getSpecularReflectance() const {
	return specularReflectance;
}

mPoint Material::getAmbientReflectance() const {
	return ambientReflectance;
}

float Material::getShininess() const {
	return shininess;
}

string Material::getFilename() const {
	return textureFilename;
}

bool Material::hasTextureFile() const {
	return isTextured;
}
