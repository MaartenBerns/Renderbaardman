#pragma once
#ifndef LIGHT_H
#define LIGHT_H

// TODO: tmp
class RayDifferential;

class Light {
public:
	void Le(const RayDifferential &ray);
};

class AreaLight: public Light {
};

#endif // LIGHT_H