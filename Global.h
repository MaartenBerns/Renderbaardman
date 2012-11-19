#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <float.h>
#include <Windows.h>
#include <math.h>
#include <algorithm>

#ifndef INFINITY
#define INFINITY FLT_MAX
#endif

#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.14159265358979323846f

class ReferenceCounted {
public:
	int nReferences;
	ReferenceCounted() { nReferences = 0; }
private:
	ReferenceCounted(const ReferenceCounted &);
	ReferenceCounted &operator=(const ReferenceCounted &);
};

template <class T> class Reference {
public:
	Reference(T *p = NULL) {
		ptr = p;
		if (ptr) ++(ptr->nReferences);
	}
	Reference(const Reference<T> &r) {
		ptr = r.ptr;
		if (ptr) ++(ptr->nReferences);
	}
	Reference &operator=(const Reference<T> &r) {
		if (r.ptr) r.ptr->nReferences++;
		if (ptr && --ptr->nReferences == 0) delete ptr;
		ptr = r.ptr;
		return *this;
	}
	Reference &operator=(T *p) {
		if (p) p->nReferences++;
		if (ptr && --ptr->nReferences == 0) delete ptr;
		ptr = p;
		return *this;
	}
	~Reference() {
		if (ptr && --ptr->nReferences == 0)
			delete ptr;
	}

	T *operator->() { return ptr; }
	const T *operator->() const { return ptr; }
	operator bool() const { return ptr != NULL; }
	bool operator<(const Reference<T> &t2) const {
		return ptr < t2.ptr;
	}
private:
	T *ptr;
};

struct Matrix4x4 : public ReferenceCounted {
public:
	float m[4][4];

	// Constructor, set to identity matrix
	Matrix4x4() {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (i == j) m[i][j] = 1.;
				else m[i][j] = 0.;
	}

	Matrix4x4(float mat[4][4]);
	Matrix4x4(float t00, float t01, float t02, float t03,
					float t10, float t11, float t12, float t13,
					float t20, float t21, float t22, float t23,
					float t30, float t31, float t32, float t33);
	
	Reference<Matrix4x4> Transpose() const;
	Reference<Matrix4x4> Inverse() const;

	static Reference<Matrix4x4>
		Mul(const Reference<Matrix4x4> &m1,
			const Reference<Matrix4x4> &m2) {
				float r[4][4];
				for (int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
						r[i][j] = m1->m[i][0] * m2->m[0][j] +
								  m1->m[i][1] * m2->m[1][j] +
								  m1->m[i][2] * m2->m[2][j] +
								  m1->m[i][3] * m2->m[3][j];
				return new Matrix4x4(r);
	}
};

// Clamp() clamps the given value val to be between the values low and high:
inline float Clamp(float val, float low, float high) {
	if (val < low) return low;
	else if (val > high) return high;
	else return val;
}

// conversion from angels expressen in degrees to radians and vice versa:
inline float Radians(float deg) {
	return (M_PI/180.f) * deg;
}

inline float Degrees(float rad) {
	return (180.f/M_PI) * rad;
}

inline bool Quadratic(float A, float B, float C, float *t0, float *t1) {
	// Find quadratic discriminant
	float discrim = B * B -4.f * A * C;
	if (discrim < 0.) return false;
	float rootDiscrim = sqrtf(discrim);
	// Compute quadratic t values
	float q;
	if (B < 0) q = -.5f * (B - rootDiscrim);
	else	   q = -.5f * (B + rootDiscrim);
	*t0 = q / A;
	*t1 = C / q;
	if (*t0 > *t1) std::swap(*t0, *t1);
	return true;
}

#endif // GLOBAL_H