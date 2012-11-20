#include "Global.h"

Reference<Matrix4x4> Matrix4x4::Transpose() const {
	return new Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0], 
						 m[0][1], m[1][1], m[2][1], m[3][1],
						 m[0][2], m[1][2], m[2][1], m[3][2], 
						 m[0][3], m[1][3], m[2][3], m[3][3]);
}

Reference<Matrix4x4> Matrix4x4::Inverse() const {
	// TODO: TO BE IMPLEMENTED
	return new Matrix4x4();	
}

Matrix4x4::Matrix4x4(float mat[4][4]) {
	// TODO: TO BE IMPLEMENTED
}

Matrix4x4::Matrix4x4(float t00, float t01, float t02, float t03,
					float t10, float t11, float t12, float t13,
					float t20, float t21, float t22, float t23,
					float t30, float t31, float t32, float t33) {
	// TODO: TO BE IMPLEMENTED
}