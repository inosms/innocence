#ifndef _ERROR_H_
#define _ERROR_H_

#include <iostream>
#include <cassert>
#include <string>

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

#define UNIMPLEMENTED std::cout << "[ERROR] Unimplemented!" << std::endl;
#define ERROR_MESSAGE(message) std::cout << "[ERROR] " << message << " " << __FILE__ << " " << __LINE__ << std::endl
#define WARNING_MESSAGE(message) std::cout << "[WARNING] " << message << " " << __FILE__ << " " << __LINE__ << std::endl


#define INFO(message) std::cout << "[INFO] " << message << std::endl
#define THROW_IF(expr,message) if(expr) throw Exception(message);

class Exception
{
	std::string m_message;
public:
	Exception(std::string n_message );
	std::string GetMessage();
};

#endif
