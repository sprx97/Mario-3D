/*
 *  CSE 40166 / 60166, Computer Graphics, Fall 2011
 *
 *  Project: generic. 
 *  File: mpoint.h
 *
 *  Description:
 *      Contains the definition of a simple point class, to make 
 *      code more compact and increase code reuse. This is not intended as a
 *      serious example of competent object-oriented programming.
 *
 *  Author:
 *      Alexandri Zavodny, University of Notre Dame.
 *  
 *  Notes:
 *      Originally written for CSE 40166/60166 in Fall 2010 and modified for 2011.
 *
 */


//header guard - make sure that disaster doesn't 
//strike if someone includes this file multiple times.
#ifndef _MPOINT_H
#define _MPOINT_H

#include <iostream>

class mPoint {

	public:

		// CONSTRUCTORS / DESTRUCTORS

		mPoint()
		{x=y=z=0.0;}

		mPoint(const float a, const float b, const float c)
		{ x = a; y = b; z = c; }

		// MISCELLANEOUS

		float magSq() const;
		float mag() const;
		void normalize();
		mPoint unit() const;
		float at(const int i);

		void glVertex();

		mPoint& operator+=(const mPoint rhs);
		mPoint& operator-=(const mPoint rhs);
		mPoint& operator*=(const float rhs);
		mPoint& operator/=(const float rhs);

		// MEMBER VARIABLES

		float x,y,z;
};

// RELATED OPERATORS

std::ostream &operator<<(std::ostream &os, const mPoint &a);
mPoint operator*(const mPoint a, const float f);
mPoint operator/(const mPoint a, const float f);
mPoint operator/(const float f, const mPoint p);
mPoint operator*(const float f, const mPoint a);
mPoint operator+(const mPoint a, const mPoint b);
mPoint operator-(const mPoint a, const mPoint b);


mPoint cross(const mPoint a, const mPoint b);
float dot(const mPoint a, const mPoint b);

#endif

