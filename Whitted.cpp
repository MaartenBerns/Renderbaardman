#include "Whitted.h"
#include "Geometry.h"
#include "Light.h"

/*
Spectrum WhittedIntegrator::Li(const Scene *scene, 
								const RayDifferential &ray, const Sample *sample, 
								float *alpha) const {
	Intersection isect;
	Spectrum L(0.);
	bool hitSomething;
	// Search for ray-primitive intersections
	hitSomething = scene->Intersect(ray, &isect);
	if (!hitSomething) {
		// Handle ray with no intersection
		if (alpha) *alpha = 0.;
		for (u_int i = 0; i < scene->lights.size(); ++i)
			L += scene->lights[i]->Le(ray);
		if (alpha && !L.Black()) * alpha = 1.;
		return L;
	}
	else {
		// Initialize alpha for ray hit
		if (alpha) *alpha = 1.;
		// Compute emitted and reflected light at ray intersection point

		// Evaluate BSD at hit point
		BSDF *bsdf = isect.GetBSDF(ray);
		// Initialize common variables for Whitted integrator
		const Point &p = bsdf->dgShading.p;
		const Normal &n = bsdf->dgShading.nn;
		Vector wo = -ray.d;
		// Compute emitted light if ray hit an area light source
		L += isect.Le(wo);
		// Add contribution of each light source
		Vector wi;
		for (u_int i = 0; i < scene->lights.size(); ++i) {
			VisibilityTester visibility;
			Spectrum Li = scene->lights[i]->Sample_L(p, &wi, &visibility);
			if (Li.Black()) continue;
			Spectrum f = bsdf->f(wo,wi);
			if (!f.Black() && visibility.Unoccluded(scene))
				L += f * Li * AbsDot(wi, n) * visibility.Transmittance(scene);
		}

		if (rayDepth++ < maxDepth) {
			// Trace rays for specular reflection and refraction
			Spectrum f = bsdf->Sample_f(wo, &wi, 
				BxDFType(BSDF_REFLECTION | BSDF_SPECULAR));
			if (!f.Black()) {
				// Compute ray differential rd for specular reflection .. TODO: 496
				L += f * scene->Li(rd, sample) * AbsDot(wi, n);
			}
			f = bsdf->Sample_f(wo, &wi, 
				BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR));
			if (!f.Black()) {
				// Compute ray differential rd for specular transmission ... TODO
				L += f * scene->Li(rd, sample) * AbsDot(wi, n);
			}
		}
		--rayDepth;
	}
} 
*/