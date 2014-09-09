#include "Camera.h"

Camera::Camera()
{
	m_matrix = glm::translate(m_matrix, glm::vec3(0,-3,-80));
}

void Camera::SetPosition( glm::vec3 n_pos )
{
	m_matrix = glm::translate(glm::mat4(1.0f),n_pos);
}

glm::mat4x4 Camera::GetMatrix()
{
	return m_matrix;
}
