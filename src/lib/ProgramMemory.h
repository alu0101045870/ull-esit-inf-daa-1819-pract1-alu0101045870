/*

*/
#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>

#include "PersExceptions.h"

// Identifier + Arg is what defines an instruction
typedef std::pair<std::string, std::string> instrucTion;

/*

*/
class ProgramMemory {
private:
	// Vector contains ordered set of instruction
	std::vector<instrucTion> instructionSet_;
	// Read labels are stored in a map. If a jump is needed, you read it from here
	std::map<std::string, unsigned int> labels_;

	// Instructions on valid format
	static const std::vector<std::string> validInstructions;

	// Private method to check instruction syntax
	bool checkInstruction(instrucTion&);

public:
	ProgramMemory(const std::string& filename);			// Initialization from .ram file
	~ProgramMemory();								// Does not do much

	instrucTion getInstruction(int ip);				// CPU may call to exec instruction at position IP
	unsigned int getInstrIndexFromLabel(const std::string&);
	std::vector<std::string> getValidInstructions();
	unsigned int size();
	// Debug mode
	std::ostream& toString(std::ostream&);
};

