#include "Primitive.h"

void Primitive::FullyRefine(std::vector<Reference<Primitive> > &refined) const {
	std::vector<Reference<Primitive> > todo;
	todo.push_back(const_cast<Primitive *>(this));
	while (todo.size()) {
		// Refine last primitive in todo list
		Reference<Primitive> prim = todo.back();
		todo.pop_back();
		if (prim->CanIntersect())
			refined.push_back(prim);
		else
			prim->Refine(todo);
	}
};

void Primitive::Refine(std::vector<Reference<Primitive> > & refined) const {
};

bool Primitive::CanIntersect() const {
	return true;
}

GeometricPrimitive::GeometricPrimitive(const Reference<Shape> &s, 
	const Reference<Material> &m, AreaLight *a) {
	shape = s;
	material = m;
	areaLight = a;
};

bool GeometricPrimitive::Intersect(const Ray &r, Intersection *isect) const {
	float thit;
	if (!shape->Intersect(r, &thit, &isect->dg))
			return false;
	isect->primitive = this;
	isect->WorldToObject = shape->WorldToObject;
	r.maxt = thit;
	return true;
};

bool GeometricPrimitive::IntersectP(const Ray &r) const {
	return true; //shape->IntersectP(r);
};

BBox GeometricPrimitive::WorldBound() const {
	return NULL;
};

bool GeometricPrimitive::CanIntersect() const {
	return true;
};

const AreaLight* GeometricPrimitive::GetAreaLight() const {
	return NULL;
};

BSDF* GeometricPrimitive::GetBSDF(const DifferentialGeometry &dg, 
		const Transform &WorldToObject) const {
	return NULL;
};
