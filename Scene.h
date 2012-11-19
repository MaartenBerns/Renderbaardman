#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Sample.h"
#include "Geometry.h"
#include "Primitive.h"
#include "Light.h"
#include "Camera.h"
#include "Spectrum.h"

using namespace std;

class Scene {
private:
public:
	// Scene Data
	Primitive * aggregate;	// Stores all the scene's primitives in an accelaration data structure
	vector<Light *> lights;
	Camera *camera;
	//VolumeRegion *volumeRegion;
	//SurfaceIntegrator *surfaceIntegrator;
	//VolumeIntegrator *volumeIntegrator;
	Sampler *sampler;

	// Render the scene
	void Render();
	// computes the radiance along the ray, and sets the alpha value for the ray
	Spectrum Li(const RayDifferential &ray, const Sample *sample, float *alpha) const;
	// computation of the attenuation along a ray due to participating meda
	Spectrum Transmittance(const Ray &ray) const;
	// traces given ray into the scene and returns a boolean value indicating whether the ray intersected any of the primitives
	//bool Intersect(const Ray &ray, Intersection *isect) const;
	// checks for intersections along the ray, but does not return any information about the intersections
	bool IntersectP(const Ray &ray) const;
};

#endif // SCENE_H