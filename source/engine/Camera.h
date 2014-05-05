#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Math.h"
#include "glm/glm/glm.hpp"

class Camera
{
private:
	glm::mat4x4 m_matrix;
	
public:

	// TODO 

	glm::mat4x4 GetMatrix();
};

#endif