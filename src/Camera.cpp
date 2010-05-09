#include "Camera.h"

Camera::Camera(void)
{
	this->pos = Vec3(0.0, -70.0, -250.0);
	
}

Camera::~Camera(void)
{
}

void Camera::move(Vec3 const &offset) {
	pos += offset;
}

void Camera::rotate(Vec3 const &offset) {
	rot += offset;
}
