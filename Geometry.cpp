#include "Geometry.h"
#include "Shape.h"

DifferentialGeometry::DifferentialGeometry(const Point &P, 
						 const Vector &DPDU, const Vector &DPDV,
						 const Vector &DNDU, const Vector &DNDV,
						 float uu, float vv, const Shape *sh)
							 : p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV) {
	// Initialize DifferentialGeometry from parameters
	nn = Normal(Normalize(Cross(dpdu, dpdv)));
	u = uu;
	v = vv;
	shape = sh;

	// Adjust normal based on orientation and handedness
	// ^ is XOR
	if (shape->reverseOrientation ^ shape->transformSwapsHandedness)
		nn * -1.f;
}
