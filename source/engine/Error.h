#ifndef _ERROR_H_
#define _ERROR_H_

#include <iostream>

#ifdef DEBUG
	#define DEBUG_MESSAGE(message) std::cout << "[DEBUG] " << message << " " << __FILE__ << " " << __LINE__ << std::endl
#else
	#define DEBUG_MESSAGE(message) 
#endif

#ifdef PEDANTIC_DEBUG
	#define PEDANTIC_DEBUG_MESSAGE(message) std::cout << "[PEDANTIC_DEBUG] " << message << " " << __FILE__ << " " << __LINE__ << std::endl
#else
	#define PEDANTIC_DEBUG_MESSAGE(message)
#endif

#define ERROR_MESSAGE(message) std::cout << "[ERROR] " << message << " " << __FILE__ << " " << __LINE__ << std::endl

#endif