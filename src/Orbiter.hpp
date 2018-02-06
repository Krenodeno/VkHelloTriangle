#ifndef ORBITER_H
#define ORBITER_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* An orbiter camera is placed on a sphere whose center is where the camera is looking at */
class Orbiter
{
public:
	Orbiter(float radius, float phy, float theta, float fov, float aspectRatio, float near = 0.1f, float far = 100.0f);
	~Orbiter();

	/*Move in direction of center of view*/
	void move(float step);
	/*Move center and eye positions*/
	void translate(glm::vec3 translation);
	/*Move the eye position around the center of view, x axis is moving right/left and y is moving up/down*/
	void rotate(glm::vec2 rotation);

	glm::mat4 view() const;
	glm::mat4 perspective() const;
	glm::mat4 ortho() const;

	glm::vec3 pos() const { return eye; }
	glm::vec3 centre() const { return center; }

private:

	/* The camera is always looking at center point, 
	   the distance between center and eye points is equal to radius */

	float radius;		// sphere radius
	float theta, phy;	// angles to determine eye position

	glm::vec3 eye,	// eye's postision (in world coordinates) - Cached value (can be calculated at every frame)
		center,		// where to look at (in world coordinates)
		up;			// direction to up (in world coordinates)

	float fov;			// in radians, like camera lens zoom
	float aspectRatio;	// width / height typically
	float near;			// 0.1 typically
	float far;			// 100.0 should be a good start 
};

#endif // ORBITER_H