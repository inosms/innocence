#include "UniqueNumberGenerator.h"

// m_lastID starts with NO_ID, as NO_ID is always smaller
// than all ids
UniqueNumberGenerator::UniqueNumberGenerator() : m_lastID(NO_ID)
{}

unsigned int UniqueNumberGenerator::GetUniqueID()
{
	return ++m_lastID;
}