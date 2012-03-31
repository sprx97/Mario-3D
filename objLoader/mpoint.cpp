/*
 *  CSE 40166 / 60166, Computer Graphics, Fall 2011
 *
 *  Project: generic. 
 *  File: mpoint.cpp
 *
 *  Description:
 *      Contains the implementation of a simple point class, to make 
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

#include "mpoint.h"
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif

// OPERATOR OVERLOADS

mPoint operator*(const mPoint a, const float f)
{
    return mPoint(a.x*f,a.y*f,a.z*f);
}

mPoint operator/(const mPoint a, const float f)
{
    return mPoint(a.x/f,a.y/f,a.z/f);
}

mPoint operator/(const float f, const mPoint a)
{
    return mPoint(a.x/f,a.y/f,a.z/f);
}

mPoint operator*(const float f, const mPoint a)
{
    return mPoint(a.x*f,a.y*f,a.z*f);
}

mPoint operator+(const mPoint a, const mPoint b)
{
    return mPoint(a.x+b.x, a.y+b.y, a.z+b.z);
}

mPoint operator-(const mPoint a, const mPoint b)
{
    return mPoint(a.x-b.x, a.y-b.y, a.z-b.z);
}


mPoint& mPoint::operator+=(const mPoint rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}


mPoint& mPoint::operator-=(const mPoint rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

mPoint& mPoint::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

mPoint& mPoint::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const mPoint &p) {
	os << p.x << ' ' << p.y << ' ' << p.z;
	return os;
}


mPoint cross(const mPoint a, const mPoint b)
{
    mPoint c;
    c.x = a.y*b.z - a.z*b.y;
    c.y = b.x*a.z - a.x*b.z;
    c.z = a.x*b.y - a.y*b.x;
    return c;
}

float dot(const mPoint a, const mPoint b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}


// MEMBER FUNCTIONS

float mPoint::magSq() const
{
    return x*x + y*y + z*z;
}

float mPoint::mag() const
{
    float t = magSq();
    if(t <= 0.0)
        return 0;
    return sqrt(t);
}

void mPoint::normalize()
{
    float m = mag();
    x /= m;
    y /= m;
    z /= m;
}

mPoint mPoint::unit() const {
	float m = mag();
	return mPoint(x / m, y / m, z / m);
}

float mPoint::at(const int i)
{
    if(i == 0)  return x;
    if(i == 1)  return y;
    if(i == 2)  return z;
    return -1;
}

void mPoint::glVertex()
{
    glVertex3f(x, y, z);
}
