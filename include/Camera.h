#pragma once
#include "Vec3.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	Vec3 pos;
	float angle_x, angle_y, angle_z;
	void move(Vec3 const &offset);
	//void rotate(float x_angle, float y_angle, float z_angle);
};
