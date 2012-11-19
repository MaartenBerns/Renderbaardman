#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Intersection.h"
#include "Global.h"
#include "Material.h"
#include "Light.h"
#include "Bounding.h"
#include "BSDF.h"

class Primitive : public ReferenceCounted {
	// returns a box that encloses the primitive's geometry in world space
	virtual BBox WorldBound() const = 0;
	virtual bool CanIntersect() const;
	virtual bool Intersect(const Ray &r, Intersection *in) const = 0;
	virtual bool IntersectP(const Ray &r) const = 0;\
	virtual const AreaLight *GetAreaLight() const = 0;
	virtual BSDF *GetBSDF(const DifferentialGeometry &dg, 
		const Transform &WorldToObject) const = 0;
	virtual void Refine(vector<Reference<Primitive> > & refined) const; // refine?
	void FullyRefine(vector<Reference<Primitive> > &refined) const;	
};

class GeometricPrimitive : public Primitive {
public:
	GeometricPrimitive(const Reference<Shape> &s, const Reference<Material> &m, AreaLight *a);
	bool Intersect(const Ray &r, Intersection *isect) const;
private:
	Reference<Shape> shape;
	Reference<Material> material;
	AreaLight *areaLight;
};

#endif // PRIMITIVE_H