#ifndef _TICKMETER_H_
#define _TICKMETER_H_

#include <string>

class Tickmeter
{
	unsigned int m_lastSecond;
	unsigned int m_ticksUntilLastSecond;
	std::string m_name;
public:
	Tickmeter(std::string n_name = "");

	void Measure();
};

#endif