#include "BBox.h"

BBox::BBox(const Point &p1, const Point &p2) {
	pMin = Point(min(p1.x, p2.x),
				min(p1.y, p2.y),
				min(p1.z, p2.z));
	pMax = Point(max(p1.x, p2.x),
				max(p1.y, p2.y),
				max(p1.z, p2.z));
}

BBox BBox::Union(const BBox &b, const Point &p) {
	BBox ret = b;
		ret.pMin.x = min(b.pMin.x, p.x);
		ret.pMin.y = min(b.pMin.y, p.y);
		ret.pMin.z = min(b.pMin.z, p.z);
		ret.pMax.x = max(b.pMax.x, p.x);
		ret.pMax.y = max(b.pMax.y, p.y);
		ret.pMax.z = max(b.pMax.z, p.z);
		return ret;
}

BBox Union(const BBox &b, const BBox &b2) {
	std::cout << "BBox::Union not implemented yet" << std::endl;
	return NULL;
}

bool BBox::Overlaps(const BBox &b) const {
	bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x);
	bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y);
	bool z = (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z);
	return (x && y && z);
}

bool BBox::Inside(const Point &pt) const {
	return (pt.x >= pMin.x && pt.x <= pMax.x &&	
			pt.y >= pMin.y && pt.y <= pMax.y &&
			pt.z >= pMin.z && pt.z <= pMax.z);
}

void BBox::Expand(float delta) {
	pMin -= Vector(delta, delta, delta);
	pMax += Vector(delta, delta, delta);
}

float BBox::Volume() const{
	Vector d = pMax - pMin;
	return d.x * d.y *d.z;
}

int BBox::MaximumExtend() const {
	Vector diag = pMax - pMin;
	if (diag.x > diag.y && diag.x > diag.z)
		return 0;
	else if (diag.y > diag.z)
		return 1;
	else
		return 2;
}

void BBox::BoundingSphere(Point *c, float *rad) const {
	*c = .5f * pMin + .5f * pMax;
	*rad = Distance(*c, pMax);
}

	
