#ifndef _INPROCESSES_H_
#define _INPROCESSES_H_

#include "../engine/Process.h"
#include <iostream>

class test_thread_process : public Thread_Process
{
public:
	void VThreadedMethod();
};

#endif