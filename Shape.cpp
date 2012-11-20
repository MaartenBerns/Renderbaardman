#include "Shape.h"

Shape::Shape(const Transform &o2w, bool ro) :
		ObjectToWorld(o2w), WorldToObject(o2w.GetInverse()), 
			reverseOrientation(ro),
				transformSwapsHandedness(o2w.SwapsHandedness()) {

}

Sphere::Sphere(const Transform &o2w, bool ro, float rad, float z0, float z1, float pm) : 
	Shape(o2w, ro) {
	radius = rad;
	zmin = Clamp(min(z0, z1), -radius, radius);
	zmax = Clamp(max(z0, z1), -radius, radius);
	thetaMin = acosf(zmin/radius);
	thetaMax = acosf(zmax/radius);
	phiMax = Radians(Clamp(pm, 0.0f, 360.0f));
}

bool Sphere::Intersect(const Ray &r, float *tHit,
					DifferentialGeometry *dg) const {
	float phi;
	Point phit;
	// Transform Ray to object space
	Ray ray;
	WorldToObject(r, &ray);
	// Computer quadratic sphere coefficients
	float A = ray.d.x*ray.d.x + ray.d.y*ray.d.y + ray.d.z*ray.d.z;
	float B = 2 * (ray.d.x*ray.o.x + ray.d.y*ray.o.y + ray.d.z*ray.o.z);
	float C = ray.o.x*ray.o.x + ray.o.y*ray.o.y +
			  ray.o.z*ray.o.z - radius*radius;
	// Solve quadratic equation for t values
	float t0, t1;
	if (!Quadratic(A, B, C, &t0, &t1))
		return false;
	// Compute intersecion distance along ray
	if (t0 > ray.maxt || t1 < ray.mint)
		return false;
	float thit = t0;
	if (t0 < ray.mint) {
		thit = t1;
		if (thit > ray.maxt) return false;
	}
	// Compute sphere hit position and phi
	phit = ray(thit);
	phi = atan2f(phit.y, phit.x);
	if (phi < 0.) phi += 2.f*M_PI;
	// Test sphere intersection agianst clipping parameters
	if (phit.z < zmin || phit.z > zmax || phi > phiMax) {
		if (thit == t1) return false;
		if (t1 > ray.maxt) return false;
		thit = t1;
		// Compute sphere hit position and phi
		phit = ray(thit);
		phi = atan2f(phit.y, phit.x);
		if (phi < 0.) phi += 2.f*M_PI;
		if (phit.z < zmin || phit.z > zmax || phi > phiMax)
			return false;
	}
	// Find parametric representation of sphere hit
	float u = phi /phiMax;
	float theta = acosf(phit.z /radius);
	float v = (theta - thetaMin) / (thetaMax - thetaMin);
	// Compute sphere dp/du and dp/dv
	float zradius = sqrtf(phit.x*phit.x + phit.y*phit.y);
	float invzradius = 1.f / zradius;
	float cosphi = phit.x * invzradius;
	float sinphi = phit.y * invzradius;
	Vector dpdu(-phiMax * phit.y, phiMax * phit.x, 0);
	Vector dpdv = (thetaMax-thetaMin) *
			Vector(phit.z * cosphi, phit.z * sinphi, 
				-radius * sinf(thetaMin + v*(thetaMax - thetaMin)));
	// Compute sphere dn/du and dn/dv
	// 'Partial Derivatives of normal vector' chapter skipped (due to its advanced nature)
	Vector d2Pduu = -phiMax * phiMax * Vector(phit.x, phit.y, 0);
	Vector d2Pduv = (thetaMax - thetaMin) * phit.z * phiMax *
						Vector(-sinphi, cosphi, 0.);
	Vector d2Pdvv = -(thetaMax - thetaMin) * (thetaMax - thetaMin) *
						Vector(phit.x, phit.y, phit.z);
	// Compute coefficients for fundamental forms
	float E = Dot(dpdu, dpdu);
	float F = Dot(dpdu, dpdv);
	float G = Dot(dpdv, dpdv);
	Vector N = Cross(dpdu, dpdv);
	float e = Dot(N, d2Pduu);
	float f = Dot(N, d2Pduv);
	float g = Dot(N, d2Pdvv);
	// Compute dn/du and dn/dv from fundamental form coefficients
	float invEGF2 = 1.f / (E*G - F*F);
	Vector dndu = (f*F - e*G) * invEGF2 * dpdu +
					(e*F - f*E) * invEGF2 * dpdv;
	Vector dndv = (g*F - f*G) * invEGF2 * dpdu +
					(f*F - g*E) * invEGF2 * dpdv;
	// Initialize DifferentialGemometry from parametric information
	*dg = DifferentialGeometry(ObjectToWorld(phit),
								ObjectToWorld(dpdu),
								ObjectToWorld(dpdv),
								ObjectToWorld(dndu),
								ObjectToWorld(dndv),
								u,v,this);
	// Update tHit for quadratic intersection
	*tHit = thit;
	return true;
}

bool Sphere::IntersectP(const Ray &r) const {
	float phi;
	Point phit;
	// Transform Ray to object space
	Ray ray;
	WorldToObject(r, &ray);
	// Computer quadratic sphere coefficients
	float A = ray.d.x*ray.d.x + ray.d.y*ray.d.y + ray.d.z*ray.d.z;
	float B = 2 * (ray.d.x*ray.o.x + ray.d.y*ray.o.y + ray.d.z*ray.o.z);
	float C = ray.o.x*ray.o.x + ray.o.y*ray.o.y +
			  ray.o.z*ray.o.z - radius*radius;
	// Solve quadratic equation for t values
	float t0, t1;
	if (!Quadratic(A, B, C, &t0, &t1))
		return false;
	// Compute intersecion distance along ray
	if (t0 > ray.maxt || t1 < ray.mint)
		return false;
	float thit = t0;
	if (t0 < ray.mint) {
		thit = t1;
		if (thit > ray.maxt) return false;
	}
	// Compute sphere hit position and phi
	phit = ray(thit);
	phi = atan2f(phit.y, phit.x);
	if (phi < 0.) phi += 2.f*M_PI;
	// Test sphere intersection agianst clipping parameters
	if (phit.z < zmin || phit.z > zmax || phi > phiMax) {
		if (thit == t1) return false;
		if (t1 > ray.maxt) return false;
		thit = t1;
		// Compute sphere hit position and phi
		phit = ray(thit);
		phi = atan2f(phit.y, phit.x);
		if (phi < 0.) phi += 2.f*M_PI;
		if (phit.z < zmin || phit.z > zmax || phi > phiMax)
			return false;
	}
	return true;
}

float Sphere::Area() const {
	return phiMax * radius * (zmax-zmin);
}

BBox Sphere::ObjectBound() const {
	return BBox(Point(-radius, -radius, zmin),
				Point(radius, radius, zmax)			
	);
}