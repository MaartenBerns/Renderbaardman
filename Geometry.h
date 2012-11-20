#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <assert.h>
#include <math.h>

#include "Global.h"

#define RAY_EPSILON 1e-3f

class Normal;

class Vector {
public: 
	float x, y, z;

	Vector(float _x=0, float _y=0, float _z=0)
		: x(_x), y(_y), z(_z) {
	}

	explicit Vector(const Normal &n);	

	// addition
	Vector operator+(const Vector &v) const {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector& operator+=(const Vector &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	// substraction
	Vector operator-(const Vector &v) const {
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector& operator-=(const Vector &v){
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	// Scaling
	Vector operator*(float f) const {
		return Vector(f*x, f*y, f*z);
	}

	Vector& operator*=(float f) {
		x *= f; y *= f; z *= f;
		return *this;
	}
	
	Vector operator/(float f) const {
		assert(f!=0);
		float inv = 1.f / f;
		return Vector(x * inv, y * inv, z * inv);
	}

	Vector &operator/=(float f){
		assert(f!=0);
		float inv = 1.f /f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	// Negation
	Vector operator-() const {
		return Vector(-x, -y, -z);
	}

	float operator[](int i) const {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	float &operator[](int i) {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	float LengthSquared() const { return x*x + y*y + z*z; }
	float Length() const { return sqrtf(LengthSquared()); }

};

class Point {
public:
	float x, y, z;
	// Constructor
	Point(float _x=0, float _y=0,float _z=0)
		: x(_x), y(_y), z(_z) {
	}

	Point operator+(const Vector &v) const {
		return Point(x + v.x, y + v.y, z + v.z);
	}

	Point &operator+=(const Vector &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Point operator+(const Point &p) const {
		return Point(x + p.x, y + p.y, z + p.z);
	}

	Point &operator+=(const Point &p) {
		x += p.x; y += p.y; z += p.z;
		return *this;
	}

	Vector operator-(const Point &p) const {
		return Vector(x - p.x, y - p.y, z - p.z);
	}

	Point operator-(const Vector &v) {
		return Point(x - v.x, y - v.y, z -v.z);
	}

	Point &operator-=(const Vector &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	Point operator/(float f) const {
		assert(f!=0);
		float inv = 1.f / f;
		return Point(x * inv, y * inv, z * inv);
	}

	Point &operator/=(float f){
		assert(f!=0);
		float inv = 1.f /f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	Point operator*(float f) const{
		return Point(x * f, y * f, z * f);
	}

	Point &operator *=(float f){
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	// TODO: Point to Point addition: page 56
};

inline Point operator*(float f, const Point &p) {
	return p*f;
}

class Normal {
public:
	float x, y, z;
	// constructor
	explicit Normal(const Vector &v)
		: x(v.x), y(v.y), z(v.z) {}	
	
	Normal(float _x=0, float _y=0, float _z=0)
		: x(_x), y(_y), z(_z) {	}

	// Scaling
	Normal operator*(float f) const {
		return Normal(f*x, f*y, f*z);
	}

	Normal& operator*=(float f) {
		x *= f; y *= f; z *= f;
		return *this;
	}
	
	Normal operator/(float f) const {
		assert(f!=0);
		float inv = 1.f / f;
		return Normal(x * inv, y * inv, z * inv);
	}

	Normal &operator/=(float f){
		assert(f!=0);
		float inv = 1.f /f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	float operator[](int i) const {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	float &operator[](int i) {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	float LengthSquared() const { return x*x + y*y + z*z; }
	float Length() const { return sqrtf(LengthSquared()); }
};

class Ray {
public:
	Point o;
	Vector d;
	mutable float mint, maxt;
	float time; // To be used for motion blur 
	
	// constructor
	Ray(): mint(RAY_EPSILON), maxt(INFINITY), time(0.f) {}
	Ray(const Point &origin, const Vector &direction, float start = RAY_EPSILON, float end = INFINITY, float t = 0.f)
		: o(origin), d(direction), mint(start), maxt(end), time(t) {}

	Point operator()(float t) const { return o + d * t; } 
};

class RayDifferential : public Ray {
public:
	bool hasDifferentials;
	Ray rx, ry;

	RayDifferential() { hasDifferentials = false; }
	RayDifferential(const Point &org, const Vector &dir)
		: Ray(org, dir) {
			hasDifferentials = false;
	}

	explicit RayDifferential(const Ray &ray) : Ray(ray) {
		hasDifferentials = false;
	}
};

class Shape;

class DifferentialGeometry {
public:
	Point p;
	Normal nn;
	float u, v;
	const Shape *shape;
	// Partial derivatives of the surface position and the surface normal
	Vector dpdu, dpdv;
	Vector dndu, dndv;

	DifferentialGeometry() { u = v = 0.; shape = NULL; }
	DifferentialGeometry(const Point &P, 
						 const Vector &DPDU, const Vector &DPDV,
						 const Vector &DNDU, const Vector &DNDV,
						 float uu, float vv, const Shape *sh);
};

inline Vector operator*(float f, const Vector &v) {
	return v*f;
}

inline float Dot(const Vector &v1, const Vector &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float AbsDot(const Vector &v1, const Vector &v2) {
	return fabsf(Dot(v1, v2));
}

inline float Dot(const Normal &n1, const Normal &n2) {
	return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
}

inline float AbsDot(const Normal &n1, const Normal &n2) {
	return fabsf(Dot(n1, n2));
}

inline Vector Cross(const Vector &v1, const Vector &v2) {
	return Vector((v1.y * v2.z) - (v1.z * v2.y),
					(v1.z * v2.x) - (v1.x * v2.z),
					(v1.x * v2.y) - (v1.y * v2.x));
}

inline Vector Normalize(const Vector &v) {
	return v / v.Length();
}

// TODO: page 54
//inline void CoordinateSystem(const Vector &v1, Vector *v2, Vector *v3) {}

inline float Distance(const Point &p1, const Point &p2) {
	return (p1- p2).Length();
}

inline float DistanceSquared(const Point &p1, const Point &p2) {
	return (p1 - p2).LengthSquared();
}

inline Vector::Vector(const Normal &n)
	: x(n.x), y(n.y), z(n.z) {}

#endif // GEOMETRY_H