/*
	File containing class DataMemory, which stores integers in an array 
	as long as the RAM is executing
*/
#pragma once

#include <map>
#include <string>

// Alias to iterate registers
typedef std::map<int, int>::iterator register_iterator;

class DataMemory {
private:
	std::map<int, int> regs_;				// Memory registers are "infinite", R(int) = int

public:
	DataMemory();							// Initializes R[0] = 0;
	~DataMemory();

	void write(int index, int value);		// Write value int register index
	int read(int index);					// Read from register index

	std::ostream& toString(std::ostream&);
};
