#ifndef _COLOR_H_
#define _COLOR_H_

#include "engine.h"
#include <vector>

class ColorInterpolator
{
	std::vector<glm::vec4> m_colors;
	// internal ticker
	float m_currentIndex = 0;
public:
	void Push(glm::vec4);

	// the smaller the slower the color changes
	void Update(float n_tickTime);

	// get the current color
	glm::vec4 Get();
};

#endif