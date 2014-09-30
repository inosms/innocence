#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Math.h"

class Camera
{
private:
	glm::mat4x4 m_matrix;

	// when goto is called the camera should go to that
	// position, but keep at THIS distance (could basically
	// be any matrix)
	glm::mat4x4 m_defaultCameraDisplacement;

	// the goal matrix, the current matrix always
	// tries to catch this
	glm::mat4x4 m_gotoMatrix;

	// a value between 0 (not adjusting)
	// and 1 (no smooth position adjusting; sudden adjusting)
	const float m_gotoSpeed = 0.05f;

public:

	Camera();

	void SetPosition( glm::vec3 n_pos );

	glm::mat4x4 GetMatrix(double n_interpolation);

	// one step further to the end position
	void Update();

	// smoothly go to this position
	void GoToPosition(glm::mat4x4 n_mat);
};

#endif
