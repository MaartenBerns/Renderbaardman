#pragma once
#ifndef BBOX_H
#define BBOX_H

#include "Geometry.h"
#include <iostream>

// uses axis-aligned bounding boxes
class BBox {
public:
	Point pMin, pMax;

	BBox() {
		pMin = Point(INFINITY, INFINITY, INFINITY);
		pMax= Point(-INFINITY, -INFINITY, -INFINITY);
	}
	BBox(const Point &p) : pMin(p), pMax(p) {}	
	BBox(const Point &p1, const Point &p2);
	BBox Union(const BBox &b, const Point &p);
	friend BBox Union(const BBox &b, const BBox &b2);
	bool Overlaps(const BBox &b) const;
	// returns if point is inside the bounding box
	bool Inside(const Point &pt) const;
	// pads the bounding box by a constant factor
	void Expand(float delta);
	// returns the volume of the bounding box
	float Volume() const;
	// return wich of the three axes is the longest (is useful, for example, when deciding
	// along wich axis to subdivide when building a kd-tree
	int MaximumExtend() const;
	//
	void BoundingSphere(Point *c, float *rad) const;

};

#endif // BBOX_H