#ifndef ORBITER_H
#define ORBITER_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Orbiter
{
public:
	Orbiter();
	Orbiter(float radius, float theta, float phy);
	Orbiter(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
	~Orbiter();

	/*Move in direction of center of view*/
	void move(float step);
	/*Move center and eye positions*/
	void translate(glm::vec3 translation);
	/*Move the eye position around the center of view*/
	void rotate(glm::vec2 rotation);

	glm::mat4 view() const;
	glm::mat4 projection(float fov, float aspect, float near, float far);

	glm::vec3 pos() const { return eye; }
	glm::vec3 centre() const { return center; }

private:

	float radius;		// sphere radius
	float theta, phy;	// angles to determine eye position

	glm::vec3 eye,	// eye's postision (in world coordinates)
		center,		// where to look at (in world coordinates)
		up;			// direction to up (in world coordinates)
};

#endif // ORBITER_H