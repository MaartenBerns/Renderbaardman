#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <vector>
#include "Global.h"
#include "Material.h"
#include "Light.h"
#include "BBox.h"
#include "BSDF.h"
#include "Shape.h"

class Primitive;

struct Intersection {
	// Intersection public methods
	DifferentialGeometry dg;
	const Primitive *primitive;
	Transform WorldToObject;
};

class Primitive : public ReferenceCounted {
	// returns a box that encloses the primitive's geometry in world space
	virtual BBox WorldBound() const = 0;
	virtual bool CanIntersect() const;
	virtual bool Intersect(const Ray &r, Intersection *in) const = 0;
	virtual bool IntersectP(const Ray &r) const = 0;
	virtual const AreaLight *GetAreaLight() const = 0;
	virtual BSDF *GetBSDF(const DifferentialGeometry &dg, 
		const Transform &WorldToObject) const = 0;
	virtual void Refine(std::vector<Reference<Primitive> > & refined) const;
	void FullyRefine(std::vector<Reference<Primitive> > &refined) const;	
};

class GeometricPrimitive : public Primitive {
public:
	GeometricPrimitive(const Reference<Shape> &s, const Reference<Material> &m, AreaLight *a);
	BBox WorldBound() const;
	bool CanIntersect() const;
	bool Intersect(const Ray &r, Intersection *isect) const;
	bool IntersectP(const Ray &r) const;
	const AreaLight *GetAreaLight() const;
	BSDF *GetBSDF(const DifferentialGeometry &dg, 
		const Transform &WorldToObject) const;	
private:
	Reference<Shape> shape;
	Reference<Material> material;
	AreaLight *areaLight;
};

#endif // PRIMITIVE_H