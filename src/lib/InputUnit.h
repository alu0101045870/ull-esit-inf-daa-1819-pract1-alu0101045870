/*
	File containing definition of class InputUnit, component whose purpose is reading 
	from a file (input stream) and sending said information to the CPU
*/
#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <vector>
#include <climits>

#include "PersExceptions.h"


/*

*/
class InputUnit {
private:
	unsigned int readingHead_;			// Specifies position to read from
	std::vector<int> inputStream_;		// Vector made to contain read-from-file input

public:

	// Constructor and Destructor
	InputUnit(const std::string& filename);	// Read file and load into inputStream + readingHead = begin
	~InputUnit();

	// readingHead_ related operations
	int read();							// Store next number and pass it to CPU -> readingHead_++ 
	void resetHead();					// readingHead_ = 0; Never used

	// Debug mode
	std::ostream& toString(std::ostream& os) const;				
	int nexToRead();
};

