#pragma once

#ifdef _WIN_32
	#pragma warning( disable : 4244 )
#endif

#include <math.h>
#define EPSILON 0.001

class Vec3 {
public:
	union {
		float v[3];
		struct {
			float x,y,z;
		};
	};

	Vec3() {
		x = y = z = 0.0;
	}

	Vec3(float x, float y, float z)
		:x(x), y(y), z(z){
		
	}

	void normalize() {
		float length = this->length();
		x /= length;
		y /= length;
		z /= length;
		if(fabs(this->length() - 1.0) > EPSILON) {
			//exception
		}
	}

	float length() {
		return sqrt(x*x + y*y + z*z);
	}

	void div(const float& div) {
		double cache = 1.0/div;
		x *= cache;
		y *= cache;
		z *= cache;
	}

	Vec3* operator+=(Vec3 const &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return this;
	}
};

inline Vec3 operator+(Vec3 const &lhs, Vec3 const &rhs) {
	return Vec3(
		lhs.x + rhs.x,
		lhs.y + rhs.y,
		lhs.z + rhs.z
		);
}

inline Vec3 operator-(Vec3 const &lhs, Vec3 const &rhs) {
	return Vec3(
		lhs.x - rhs.x,
		lhs.y - rhs.y,
		lhs.z - rhs.z
		);
}

inline Vec3 operator*(Vec3 const &lhs, double const &rhs) {
	return Vec3(
		lhs.x * rhs,
		lhs.y * rhs,
		lhs.z * rhs
		);
}

inline Vec3 cross(Vec3 const &lhs, Vec3 const &rhs) {
	return Vec3(
		(lhs.y*rhs.z - lhs.z*rhs.y),
		(lhs.z*rhs.x - lhs.x*rhs.z),
		(lhs.x*rhs.y - lhs.y*rhs.x)
		);
}