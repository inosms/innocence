#include "Color.h"

void ColorInterpolator::Push(glm::vec3 n_color)
{
	m_colors.push_back(n_color);
}

void ColorInterpolator::Update(float n_tickTime)
{
	m_currentIndex += n_tickTime;

	if( m_currentIndex >= m_colors.size() ) 
		m_currentIndex -= m_colors.size();
}

glm::vec3 ColorInterpolator::Get()
{
	if( m_colors.size() <= 1 ) m_colors[0];

	unsigned int tmp_color1Index = int(m_currentIndex);
	unsigned int tmp_color2Index = tmp_color1Index+1;
	if( tmp_color2Index >= m_colors.size() ) 
		tmp_color2Index -= m_colors.size();

	glm::vec3 tmp_1Color = m_colors[tmp_color1Index];
	glm::vec3 tmp_2Color = m_colors[tmp_color2Index];

	float tmp_color2Factor = m_currentIndex - int(m_currentIndex);
	float tmp_color1Factor = 1-tmp_color2Factor;

	return tmp_1Color * tmp_color1Factor + tmp_2Color * tmp_color2Factor;
}