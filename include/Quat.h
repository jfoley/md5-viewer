#pragma once
#include "Vec3.h"

class Quat {
public:
	union {
		float c[4];
		struct {
			float x,y,z,w;
		};
	};
	Quat() {
		x = y = z = w = 0.0;
	}
	Quat(float x, float y, float z)
	:x(x), y(y), z(z) {
		calculate_W();
	}

	Quat(float x, float y, float z, float w)
		:x(x), y(y), z(z), w(w) {
	}

	Quat(Vec3 vec) {
		x = vec.x; y = vec.y; z = vec.z; w = 0.0;
	}

	inline void calculate_W() {
		float t = 1.0 - x*x - y*y - z*z;
		w = (t < 0.0) ? 0.0 : -sqrtf(t);
	}

	inline void assign(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		calculate_W();
	}

	inline void assign(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Quat operator*(const Quat& rhs) const {
		Quat r;

		r.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		r.y = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
		r.z = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;
		r.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		return r;
	}

	Vec3 operator*(const Vec3& vec) const {
		Quat quat = *this * Quat(vec) * conjugate();
		return Vec3(quat.x, quat.y, quat.z);
	}

	inline Quat conjugate() const { return Quat(-x, -y, -z,	w); }

	inline void normalize() {
		float mag = sqrt (x*x + y*y	+ z*z + w*w);
		
		/* check for bogus length, to protect against divide by zero */
		if (mag > 0.0f)
		{
			/* normalize it */
			double oneOverMag = 1.0f / mag;
			
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
			w *= oneOverMag;
		}
	}
};