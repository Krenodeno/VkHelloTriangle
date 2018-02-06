#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* A free fly camera is flying around at will */
class FlyCamera
{
public:
	FlyCamera();
	~FlyCamera();

	/* step is in screen coordinates, x axis is right/left, y is up/down, z is forward/backward*/
	void move(glm::vec3 step);
	/* step is in world coordinates */
	void translate(glm::vec3 step);
	/* rotating the camera on itself, rotation is in screen coordinates */
	void rotate(glm::vec2 rotation);

	glm::mat4 view() const;
	glm::mat4 perspective() const;
	glm::mat4 ortho() const;

private:
	float pitch,	// how much you are looking up or down
		yaw,		// how much you are looking left or right
		roll;		// how much you are rolling your view

	glm::vec3 eye,	// eye's postision (in world coordinates)
		center,		// where to look at (in world coordinates) - Cached var
		up;			// direction to up (in world coordinates)

	float fov;			// in radians, like camera lens zoom
	float aspectRatio;	// width / height typically
	float near;			// 0.1 typically
	float far;			// 100.0 should be a good start 
};

#endif // FLY_CAMERA_H