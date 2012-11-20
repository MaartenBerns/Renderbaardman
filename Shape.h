#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>

#include "Geometry.h"
#include "Global.h"
#include "Transform.h"
#include <vector>
#include "BBox.h"

class Shape : public ReferenceCounted {
public:
	// Shape public data
	const Transform ObjectToWorld, WorldToObject;
	const bool reverseOrientation, transformSwapsHandedness;
	// Shape interface
	Shape(const Transform &o2w, bool ro);
	virtual bool CanIntersect() const { return true; }
	virtual bool Intersect(const Ray &ray, float *tHit, DifferentialGeometry *dg) const {
		std::cout << "Unimplemented Shape::Intersect method called" << std::endl;
		return false;
	}
	virtual bool IntersectP(const Ray &ray) const {
		std::cout << "Unimplemented Shape::IntersectP method called" << std::endl;
		return false;
	}
	virtual void GetShadingGeometry(const Transform &obj2world,
		const DifferentialGeometry &dg,
		DifferentialGeometry *dgshading) const {
		*dgshading = dg;
	}
	// compute the area of a shape in object space
	virtual float Area() const {
		std::cout << "unimplemented Shape::Area() method called" << std::endl;
		return 0.;
	}
	virtual void Refine(std::vector<Reference<Shape> > &refined) const {
		std::cout << "Unimplemented Shape::Refine() method called" << std::endl;
	}
	virtual BBox ObjectBound() const = 0;
	virtual BBox WorldBound() const {
		return ObjectToWorld(ObjectBound());
	}
};

class Sphere : public Shape {
public:
	// constructor
	Sphere(const Transform &o2w, bool ro, float rad, float z0, float z1, float pm);
	bool Intersect(const Ray &r, float *tHit, DifferentialGeometry *dg) const;
	bool IntersectP(const Ray &r) const;
	float Area() const;
	BBox ObjectBound() const;

private:
	float radius;
	float phiMax;
	float zmin, zmax;
	float thetaMin, thetaMax;
};

#endif // SHAPE_H