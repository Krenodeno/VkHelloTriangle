#include "Orbiter.hpp"


Orbiter::Orbiter(float radius, float phy, float theta, float fov, float aspectRatio, float near, float far)
	: radius(radius), theta(theta), phy(phy), center(0.0f, 0.0f, 0.0f), up(0.0f, 0.0f, 1.0f),
	fov(fov), aspectRatio(aspectRatio), near(near), far(far)
{
	move(0.0f);
}

Orbiter::~Orbiter()
{
}

void Orbiter::move(float step)
{
	radius -= step;

	eye = center;

	/*eye.x += radius * glm::cos(phy) * glm::cos(theta);
	eye.y += radius * glm::cos(phy) * glm::sin(theta);
	eye.z += radius * glm::sin(phy);*/

	eye.x += radius * glm::sin(phy) * glm::cos(theta);
	eye.y += radius * glm::sin(phy) * glm::sin(theta);
	eye.z += radius * glm::cos(phy);
}

void Orbiter::translate(glm::vec3 translation)
{
	center += translation;

	move(0.0f);
}

void Orbiter::rotate(glm::vec2 rotation)
{
	theta += rotation.x * 0.1f;
	phy -= rotation.y * 0.1f;

	move(0.0f);
}

glm::mat4 Orbiter::view() const
{
	return glm::lookAt(eye, center, up);
}

glm::mat4 Orbiter::perspective() const
{
	return glm::perspective(fov, aspectRatio, near, far);
}

glm::mat4 Orbiter::ortho() const
{
	return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
}
