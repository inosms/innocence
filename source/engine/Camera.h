#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Math.h"

class Camera
{
private:
	glm::mat4x4 m_matrix;
	
public:

	Camera();

	void SetPosition( glm::vec3 n_pos );

	glm::mat4x4 GetMatrix();
};

#endif