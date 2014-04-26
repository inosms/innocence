#include "inProcesses.h"
#include <iostream>

void test_thread_process::VThreadedMethod()
{
	int i = 0;
	while( m_running )
	{
		if( (++i)%123000000==0)
		{
			std::cout << i << std::endl;
		}
	}
		std::cout << "END DEAD" << std::endl;
}