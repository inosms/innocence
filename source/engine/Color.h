#ifndef _COLOR_H_
#define _COLOR_H_

#include "engine.h"
#include <vector>

class ColorInterpolator
{
	std::vector<glm::vec3> m_colors;
	// internal ticker
	float m_currentIndex = 0;
public:
	void Push(glm::vec3);

	// the smaller the slower the color changes
	void Update(float n_tickTime);

	// get the current color
	glm::vec3 Get();
};

#endif