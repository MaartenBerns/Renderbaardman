#pragma once
#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Primitive.h"
#include "Geometry.h"
#include "Transform.h"

struct Intersection {
	// Intersection public methods
	DifferentialGeometry dg;
	const Primitive *primitive;
	Transform WorldToObject;
};

#endif INTERSECTION_H