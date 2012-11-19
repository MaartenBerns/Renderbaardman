#include "Primitive.h"

void Primitive::FullyRefine(vector<Reference<Primitive> > &refinded) const {
	vector(Reference<Primitive> > todo;
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