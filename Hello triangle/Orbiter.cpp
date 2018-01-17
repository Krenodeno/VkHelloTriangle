#include "Orbiter.hpp"



//Orbiter::Orbiter() : Orbiter(glm::vec3(2.f, 2.f, 2.f), glm::vec3(), glm::vec3(0.f, 0.f, 1.f))
//{
//}

Orbiter::Orbiter() : Orbiter(8.0f, glm::radians(45.0f)*glm::pi<float>(), glm::radians(45.0f)*glm::pi<float>())
{
}

Orbiter::Orbiter(float radius, float theta, float phy)
	: radius(radius), theta(theta), phy(phy), center(glm::vec3(0.0f, 0.0f, 0.0f)), up(0.0f, 0.0f, 1.0f)
{
	move(0.0f);
}


Orbiter::Orbiter(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
	: eye(eye), center(center), up(up)
{
}

Orbiter::~Orbiter()
{
}

void Orbiter::move(float step)
{
	/*auto dir = glm::normalize(eye - center);
	eye += dir * step * 0.01f * glm::distance(eye, center);
	if (glm::distance(center, eye) < 0.1f)
		eye = dir * 0.1f;*/

	radius -= step;

	eye = center;

	eye.x += radius * glm::cos(phy) * glm::cos(theta);
	eye.y += radius * glm::cos(phy) * glm::sin(theta);
	eye.z += radius * glm::sin(phy);
}

void Orbiter::translate(glm::vec3 translation)
{
	/*eye += translation;
	center += translation;*/

	center += translation;

	move(0.0f);
}

void Orbiter::rotate(glm::vec2 rotation)
{
	/*glm::vec4 rotWorld = glm::inverse(view()) * glm::inverse(projection(glm::radians(45.0f), 4/3, 0.1f, 1000.0f)) * glm::vec4(rotation, 0.0f, 0.0f);

	auto size = glm::distance(eye, center);
	eye.x += rotWorld.x;
	eye.y += rotWorld.y;
	eye.z += rotWorld.z;
	auto normalizedDir = glm::normalize(eye - center);

	eye = normalizedDir * size;*/

	theta += rotation.x * 0.1f;
	phy -= rotation.y * 0.1f;

	move(0.0f);
}

glm::mat4 Orbiter::view() const
{
	return glm::lookAt(eye, center, up);
}

glm::mat4 Orbiter::projection(float fov, float aspect, float near, float far)
{
	return glm::perspective(fov, aspect, near, far);
}
