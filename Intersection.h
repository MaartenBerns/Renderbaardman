#include "Geometry.h"
#include "Transform.h"
#include "Primitive.h"

struct Intersection {
	// Intersection public methods
	DifferentialGeometry dg;
	const Primitive *primitive;
	Transform WorldToObject;
}