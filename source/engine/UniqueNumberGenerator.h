#ifndef _UNIQUE_NUMBER_GENERATOR_H_
#define _UNIQUE_NUMBER_GENERATOR_H_

// This is a very dirty unique number generator; it just increments an initial startvalue
class UniqueNumberGenerator
{
private:
	// this stores the last used unique id given
	unsigned int m_lastID;

public:

	// all other IDs are larger than this NO_ID; 
	// the value of NO_ID is used, when you want to make sure, that this
	// particualar object does not yet have a defined ID
	static const unsigned int NO_ID = 0;

	UniqueNumberGenerator();

	unsigned int GetUniqueID();
};

#endif