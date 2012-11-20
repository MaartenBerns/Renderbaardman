#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Geometry.h"
#include "Global.h"
#include "BBox.h"

// TODO:
class Vector;
class Point;
class Normal;
class Ray;

class Transform {
public:
	Reference<Matrix4x4> m, mInv;
	// constructor
	Transform();
	Transform(float mat[4][4]);
	Transform(const Reference<Matrix4x4> &mat,
					const Reference<Matrix4x4> &minv);
	Transform GetInverse() const;

	// The transformations
	Transform Translate(const Vector &delta);
	Transform Scale(float x, float y, float z);
	Transform RotateX(float angle);
	Transform RotateY(float angle);
	Transform RotateZ(float angle);
	Transform Rotate(float angle, const Vector &axis);
	// Lookat transform, used for placing the camera in the scene
	Transform LookAt(const Point &pos, const Point &look, const Vector &up);

	// Transformations on points
	Point operator()(const Point &pt) const;
	void operator()(const Point &pt, Point *ptrans) const;

	// Transformations on vectors
	Vector operator()(const Vector &v) const;
	void operator()(const Vector &v, Vector *vtrans) const;

	// Transformations on Normals
	Normal operator()(const Normal &n) const;
	void operator()(const Normal &n, Normal *ntrans) const;

	// Transformation of Rays
	Ray operator()(const Ray &r) const;
	void operator()(const Ray &r, Ray *rtrans) const;

	// Transformation on Bounding Box
	BBox operator()(const BBox &b) const;

	// Composition of Transformations
	Transform operator*(const Transform &t2) const;

	bool SwapsHandedness() const;
private:
};

#endif // TRANSFORM_H

