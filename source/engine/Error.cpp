#include "Error.h"

Exception::Exception(std::string n_message ) :
	m_message(n_message)
	{
		ERROR_MESSAGE(m_message);
	}

std::string Exception::GetMessage()
{
	return m_message;
}