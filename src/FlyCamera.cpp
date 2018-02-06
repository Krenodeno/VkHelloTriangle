#include "FlyCamera.hpp"



FlyCamera::FlyCamera()
	: pitch(0.0f), yaw(0.0f), roll(0.0f), eye(2.0f, 2.0f, 2.0f), center(0.0f, 0.0f, 0.0f), up(0.0f, 0.0f, 1.0f),
	fov(glm::radians(45.0f)), aspectRatio(4/3), near(0.1f), far(100.0f)
{
}


FlyCamera::~FlyCamera()
{
}

void FlyCamera::move(glm::vec3 step)
{
	glm::vec3 front(center - eye);
	eye += front * step;
	center += front * step;
}

void FlyCamera::translate(glm::vec3 step)
{
	eye += step;
	center += step;
}

void FlyCamera::rotate(glm::vec2 rotation)
{
	yaw += rotation.x;
	pitch += rotation.y;
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	center = eye + glm::normalize(front);
}

glm::mat4 FlyCamera::view() const
{
	return glm::lookAt(eye, center, up);
}

glm::mat4 FlyCamera::perspective() const
{
	return glm::perspective(fov, aspectRatio, near, far);
}

glm::mat4 FlyCamera::ortho() const
{
	return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
}
