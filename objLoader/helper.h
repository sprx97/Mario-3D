/*****************************************************
** CSE 40166 Computer Graphics
** Homework 06 -- Shader Programming
** helper.h
** 22 November 2011
** Josh Wise
******************************************************/

#ifndef _HELPER_H_
#define _HELPER_H_

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

#include "mPoint.h"

#include <cmath>
#include <cstdio>
#include <cstring>

#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

using std::string;
using std::vector;

// Texturing helpers
void renderGLUTText(float, float, const char*, mPoint);
bool readTGA(const std::string filename, int &imageWidth, int &imageHeight, unsigned char* &imageData, bool &wasRGBA);
bool registerOpenGLTexture(unsigned char *imageData, const int texWidth, const int texHeight, GLuint &texHandle);
vector<string> tokenizeString(const string input, const string delimiters);
#endif

