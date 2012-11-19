#include "Scene.h"
#include "Integrator.h"

class WhittedIntegrator : public SurfaceIntegrator {
public:
	Spectrum Li(const Scene *scene, 
				const RayDifferential &ray, const Sample *sample, 
				float *alpha) const;
private:
	int maxDepth;
	mutable int rayDepth;
};