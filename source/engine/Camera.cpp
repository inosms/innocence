#include "Camera.h"

Camera::Camera()
{
	m_matrix = glm::translate(m_matrix, glm::vec3(0,0,-100));
	GoToPosition(m_matrix);
}

void Camera::SetPosition( glm::vec3 n_pos )
{
	m_matrix = glm::translate(glm::mat4(1.0f),n_pos);
}

glm::mat4x4 Camera::GetMatrix(double n_interpolation)
{
	glm::mat4 tmp_diff = m_gotoMatrix - m_matrix;
	return m_matrix + m_gotoSpeed*float(n_interpolation)*tmp_diff;
}

void Camera::Update()
{
	glm::mat4 tmp_diff = m_gotoMatrix - m_matrix;
	// to to goto position, but don't go there directly.
	// instead go there slowly/fast depending on m_gotoSpeed
	// the lower, the smoother/slower the camera
	m_matrix += m_gotoSpeed*tmp_diff;
}

void Camera::GoToPosition(glm::mat4x4 n_mat)
{
	m_gotoMatrix = n_mat * glm::translate(glm::mat4(1.f), glm::vec3(0,0,-100));
}
