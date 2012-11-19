#pragma once
#ifndef LIGHT_H
#define LIGHT_H

class Light {
public:
	void Le(const RayDifferential &ray);
};

class AreaLight: public Light {
};

#endif // LIGHT_H