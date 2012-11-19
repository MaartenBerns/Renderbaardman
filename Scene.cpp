#include "Scene.h"

void Scene::Render() {
	/*
	// Allocate and initialize sample
	Sample *sample = new Sample(surfaceIntegrator, volumeIntegrator, this);
	// Allow integrators to do pre-processing for the scene
	surfaceIntegrator->Preprocess(this);
	volumeIntegrator->Preprocess(this);
	// Trace rays: the main loop
	ProgressReporter progress(sampler->TotalSamples(), "Rendering");
	while (sampler->GetNextSample(sample)) {
		// Find camera ray for sample
		RayDifferential ray; // Raydifferential is the class used to represent the camera ray
		float rayWeight = camera->GenerateRay(*sample, &ray);
			
		// Generate ray differentials for camera ray
		++(sample->imageX);
		camera->GenerateRay(*sample, &ray.rx);  // RayDifferential class inherits from ray and holds neighbouring ray.rx and ray.ry information
		--(sample->imageX);
		++(sample->imageY);
		camera->GenerateRay(*sample, &ray.ry);
		ray.hasDifferentials = true;
		--(sample->imageY);

		// Evaluate radiance along camera ray
		float alpha;
		Spectrum Ls = 0.f;
		if (rayWeight > 0.f)
			Ls = rayWeight * Li(ray, sample, &alpha);
		// Issue warning if unexpected radiance value returned

		// Add sample contribution to image
		camera->film->AddSample(*sample, ray, Ls, alpha);

		// Free BSDF memory from computing image sample value
		BSDF::FreeAll();

		// Report rendering progress
		static StatsCounter cameraRaysTraced("Camera", "Camera Rays Traced");
		++cameraRaysTraced;
		progress.Update();
	}
	// Clean up after rendering and store final image
	delete sample;
	progress.Done();
	camera->film->WriteImage();
	*/
}
/*
Spectrum Scene::Li(const RayDifferential &ray, 
						const Sample *sample, float *alpha) const {
		// outgoing radiance Lo, from the first surface that the ray intersects 
		Spectrum Lo = SurfaceIntegrator->Li(this, ray, sample, alpha);
		// the fraction of light T that is extinguished between the surface and the camera
		Spectrum T = volumeIntegrator->Transmittance(this, ray, sample, alpha);
		// the radiance Lv added along the ray due to ineractions with participating media
		Specturm lV = volumeIntegrator->Li(this, ray, sample, alpha);
		return T * Lo + Lv;
}*/

/*
Spectrum Scene::Transmittance(const Ray &ray) const {	
	return volumeIntegrator->Transmittance(this, ray, NULL, NULL);
}*/

/*
bool Scene::Intersect(const Ray &ray, Intersection *isect) const {
	return aggregate->Intersect(ray, isect);	
}
*/

bool Scene::IntersectP(const Ray &ray) const {
	//return aggregate->IntersectP(ray);
	return false;
}
