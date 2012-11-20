#include "Transform.h"
#include <math.h>

Transform::Transform() {
	m = mInv = new Matrix4x4;
}

Transform::Transform(float mat[4][4]) {
	m = new Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
					  mat[1][0], mat[1][1], mat[1][2], mat[1][3],
					  mat[2][0], mat[2][1], mat[2][2], mat[2][3],
					  mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
	mInv = m->Inverse();
}

Transform::Transform(const Reference<Matrix4x4> &mat,
					const Reference<Matrix4x4> &minv) {
	m = mat;
	mInv = minv;
}

Transform Transform::GetInverse() const {
	return Transform(mInv, m);
}

Transform Transform::Translate(const Vector &delta) {
	Matrix4x4 *m, *minv;
	m = new Matrix4x4(1, 0, 0, delta.x,
					  0, 1, 0, delta.y,
					  0, 0, 0, delta.z,
					  0, 0, 0, 1);
	minv = new Matrix4x4(1, 0, 0, -delta.x,
						 0, 1, 0, -delta.y,
						 0, 0, 0, -delta.z,
						 0, 0, 0, 1);
	return Transform(m, minv);
}

Transform Transform::Scale(float x, float y, float z) {
	Matrix4x4 *m, *minv;
	m = new Matrix4x4(x, 0, 0, 0,
					  0, y, 0, 0,
					  0, 0, z, 0,
					  0, 0, 0, 1);
	minv = new Matrix4x4(1.f/x, 0, 0, 0,
						 0, 1.f/y, 0, 0,
						 0, 0, 1.f/z, 0,
						 0, 0, 0, 1);
	return Transform(m, minv);
}

Transform Transform::RotateX(float angle) {
	float sin_t = sinf(Radians(angle)); 
	float cos_t = cosf(Radians(angle));
	Matrix4x4 *m = new Matrix4x4(1, 0, 0, 0,
						 		0, cos_t, -sin_t, 0,
								0, sin_t, cos_t, 0,
								0, 0, 0, 1);
	return Transform(m, m->Transpose());
}

Transform Transform::RotateY(float angle) {
	float sin_t = sinf(Radians(angle));  
	float cos_t = cosf(Radians(angle));
	Matrix4x4 *m = new Matrix4x4(cos_t, 0, sin_t, 0,
						 		 0, 1, 0, 0,
								 -sin_t, 0, cos_t, 0,
								 0, 0, 0, 1);
	return Transform(m, m->Transpose());
}

Transform Transform::RotateZ(float angle) {
	float sin_t = sinf(Radians(angle));  
	float cos_t = cosf(Radians(angle));
	Matrix4x4 *m = new Matrix4x4(cos_t, -sin_t, 0, 0,
						 		 sin_t, cos_t, 0, 0,
								 0, 0, 1, 0,
								 0, 0, 0, 1);
	return Transform(m, m->Transpose());
}

Transform Transform::Rotate(float angle, const Vector &axis) {
	Vector a = Normalize(axis);
	float s = sinf(Radians(angle));
	float c = cosf(Radians(angle));
	float m[4][4];

	m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
	m[0][0] = a.x * a.y * (1.f - c) - a.z * s;
	m[0][0] = a.x * a.z * (1.f - c) + a.y * s;
	m[0][0] = 0;

	m[0][0] = a.x * a.y * (1.f - c) + a.z * s;
	m[0][0] = a.y * a.y + (1.f - a.y * a.y) * c;
	m[0][0] = a.y * a.z * (1.f - c) - a.x * s;
	m[0][0] = 0;

	m[0][0] = a.x * a.z * (1.f - c) - a.y * s;
	m[0][0] = a.y * a.z * (1.f - c) + a.x * s;
	m[0][0] = a.z * a.z + (1.f - a.z * a.z) * c;
	m[0][0] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	Matrix4x4 *mat = new Matrix4x4(m);
	return Transform(mat, mat->Transpose());
}

Transform Transform::LookAt(const Point &pos, const Point &look, const Vector &up) {
	float m[4][4];
	
	// Initialize fourth column of viewing matrix
	m[0][3] = pos.x;
	m[1][3] = pos.y;
	m[2][3] = pos.z;
	m[3][3] = 1;
	
	// Initialize first three columns of vieuwing matrix
	Vector dir = Normalize(look - pos);
	Vector right = Cross(dir, Normalize(up));
	Vector newUp = Cross(right, dir);
	m[0][0] = right.x;
	m[1][0] = right.y;
	m[2][0] = right.z;
	m[3][0] = 0;
	m[0][1] = newUp.x;
	m[1][1] = newUp.y;
	m[2][1] = newUp.z;
	m[3][1] = 0;
	m[0][2] = dir.x;
	m[1][2] = dir.y;
	m[2][2] = dir.z;
	m[3][2] = 0;

	Matrix4x4 *camToWorld = new Matrix4x4(m);
	return Transform(camToWorld->Inverse(), camToWorld);
}

inline Point Transform::operator()(const Point &pt) const {
	float x = pt.x, y = pt.y, z  = pt.z;
	float xp = m->m[0][0]*x + m->m[0][1]*y + m->m[0][2]*z + m->m[0][3];
	float yp = m->m[1][0]*x + m->m[1][1]*y + m->m[1][2]*z + m->m[1][3];
	float zp = m->m[2][0]*x + m->m[2][1]*y + m->m[2][2]*z + m->m[2][3];
	float wp = m->m[3][0]*x + m->m[3][1]*y + m->m[3][2]*z + m->m[3][3];

	assert(wp != 0);
	if (wp == 1.) return Point(xp, yp, zp);
	else		  return Point(xp, yp, zp) / wp;
}

inline void Transform::operator()(const Point &pt, Point *ptrans) const {
	float x = pt.x, y = pt.y, z = pt.z;
	ptrans->x= m->m[0][0]*x + m->m[0][1]*y + m->m[0][2]*z + m->m[0][3];
	ptrans->y = m->m[1][0]*x + m->m[1][1]*y + m->m[1][2]*z + m->m[1][3];
	ptrans->z = m->m[2][0]*x + m->m[2][1]*y + m->m[2][2]*z + m->m[2][3];
	float w = m->m[3][0]*x + m->m[3][1]*y + m->m[3][2]*z + m->m[3][3];
	if (w != 1.) * ptrans /= w;
}

inline Vector Transform::operator()(const Vector &v) const {
	float x = v.x, y = v.y, z = v.z;
	return Vector(m->m[0][0]*x + m->m[0][1]*y + m->m[0][2]*z,
				  m->m[1][0]*x + m->m[1][1]*y + m->m[1][2]*z,
				  m->m[2][0]*x + m->m[2][1]*y + m->m[2][2]*z);
}

inline void Transform::operator()(const Vector &v, Vector *vtrans) const {
	float x = v.x, y = v.y, z = v.z;
	vtrans->x = m->m[0][0]*x + m->m[0][1]*y + m->m[0][2]*z;
	vtrans->y = m->m[1][0]*x + m->m[1][1]*y + m->m[1][2]*z;
	vtrans->z = m->m[2][0]*x + m->m[2][1]*y + m->m[2][2]*z;
}

inline Normal Transform::operator()(const Normal &n) const {
	float x = n.x, y = n.y, z = n.z;
	return Normal(mInv->m[0][0]*x + mInv->m[1][0]*y + mInv->m[2][0]*z, 
			      mInv->m[0][1]*x + mInv->m[1][1]*y + mInv->m[2][1]*z, 
				  mInv->m[0][2]*x + mInv->m[1][2]*y + mInv->m[2][2]*z);
}

inline void Transform::operator()(const Normal &n, Normal *ntrans) const {
	float x = n.x, y = n.y, z = n.z;
	ntrans->x = mInv->m[0][0]*x + mInv->m[1][0]*y + mInv->m[2][0]*z;
	ntrans->y = mInv->m[0][1]*x + mInv->m[1][1]*y + mInv->m[2][1]*z;
	ntrans->z = mInv->m[0][2]*x + mInv->m[1][2]*y + mInv->m[2][2]*z;
}

inline Ray Transform::operator()(const Ray &r) const {
	Ray ret;
	(*this)(r.o, &ret.o);
	(*this)(r.d, &ret.d);
	ret.mint = r.mint;
	ret.maxt = r.maxt;
	ret.time = r.time;
	return ret;
}

inline void Transform::operator()(const Ray &r, Ray *rtrans) const {
	// TODO: to be implemented
}

Transform Transform::operator*(const Transform &t2) const {
	Reference<Matrix4x4> m1 = Matrix4x4::Mul(m, t2.m);
	Reference<Matrix4x4> m2 = Matrix4x4::Mul(t2.mInv, mInv);
	return Transform(m1, m2);
}

bool Transform::SwapsHandedness() const {
	// the handedness of the coordinate system is changed when
	// the determinant of the transformation's upper-left 3x3 
	// matrix is negative
	float det = ((m->m[0][0] *
				(m->m[1][1] * m->m[2][2] -
				 m->m[1][2] * m->m[2][1])) -
				 (m->m[0][1] *
				 (m->m[1][0] * m->m[2][2] -
				  m->m[1][2] * m->m[2][0])) +
				  (m->m[0][2] *
				  (m->m[1][0] * m->m[2][1] -
				  m->m[1][1] * m->m[2][0])));
	return det < 0.f;
}

BBox Transform::operator()(const BBox &b) const {
	const Transform &M = *this;
	BBox ret(		 M(Point(b.pMin.x, b.pMin.y, b.pMin.z)));
	ret = Union(ret, M(Point(b.pMax.x, b.pMin.y, b.pMin.z)));
	ret = Union(ret, M(Point(b.pMin.x, b.pMax.y, b.pMin.z)));
	ret = Union(ret, M(Point(b.pMin.x, b.pMin.y, b.pMax.z)));
	ret = Union(ret, M(Point(b.pMin.x, b.pMax.y, b.pMax.z)));
	ret = Union(ret, M(Point(b.pMax.x, b.pMax.y, b.pMin.z)));
	ret = Union(ret, M(Point(b.pMax.x, b.pMin.y, b.pMax.z)));
	ret = Union(ret, M(Point(b.pMax.x, b.pMax.y, b.pMax.z)));
	return ret;
}
