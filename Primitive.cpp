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
	return shape->IntersectP(r);
};

BBox GeometricPrimitive::WorldBound() const {
	return shape->WorldBound();	
};

bool GeometricPrimitive::CanIntersect() const {
	return shape->CanIntersect();
};

const AreaLight* GeometricPrimitive::GetAreaLight() const {
	return areaLight;
};

BSDF* GeometricPrimitive::GetBSDF(const DifferentialGeometry &dg, 
		const Transform &WorldToObject) const {
	DifferentialGeometry dgs;
	shape->GetShadingGeometry(WorldToObject.GetInverse(), dg, &dgs);
	return material->GetBSDF(dg, dgs);
};

bool InstancePrimitive::Intersect(const Ray &r, Intersection *isect) const {
	Ray ray = WorldToInstance(r);
	if (!instance->Intersect(ray, isect))
		return false;
	r.maxt = ray.maxt;
	isect->WorldToObject = isect->WorldToObject * WorldToInstance;
	// TODO: transform instance's differential geometry to world space
	return true;
}