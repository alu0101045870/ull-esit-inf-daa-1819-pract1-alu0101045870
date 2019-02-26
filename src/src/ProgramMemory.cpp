#include "../lib/ProgramMemory.h"

//---------------------------PUBLIC MEMBERS--------------------------------

ProgramMemory::ProgramMemory(const std::string& filename) {
	//This is going to be tricky
	std::ifstream infile;
	infile.exceptions(std::ifstream::badbit);	//throw failure automatically when error occurs
	infile.open(filename);

	if (!infile.good()) {
		//Error handling
		std::stringstream errorString("Error en la apertura del archivo: ");
		errorString << filename;
		throw std::ifstream::failure(errorString.str());
	}
	else {
		// Line, and three tokens for instructions
		std::string input, tok1, tok2, tok3;
		// Regular expressions, #.* and 
		std::regex almh("(#)(.*)"), lab("(.*)(:)");
		// Instruction count
		int insCount = 0;
		// Size for label token
		unsigned labelSize;
		// Line count
		int linecount = 1;

		while (std::getline(infile, input)) {
			
			// allows to process the line as a set of tokens
			std::istringstream iss(input);
			// Is the line empty?
			if ((iss >> tok1)) {

				// Is the line a commentary ?
				if (std::regex_match(tok1, almh)) {}
				// Does it have a label ?
				else if (std::regex_match(tok1, lab)) {
					labelSize = tok1.size();
					tok1.resize(labelSize-1);			//Cut the ':'
					// store it in map 
					labels_[tok1] = insCount;
					// now store the instruction
					
					if ((iss >> tok2)) {		// If instruction is found
						// Does it have an argument?
						if (iss >> tok3) {
							instrucTion instruction(tok2, tok3);
							if (checkInstruction(instruction)) {
								instructionSet_.push_back(instruction);
								insCount++;
							}
							else {
								throw CompileTimeErrorException(linecount);
							}
						}
						else {
							tok3 = "";
							// There are instructions with a single operator
							instrucTion instruction(tok2, tok3);
							if (checkInstruction(instruction)) {
								instructionSet_.push_back(instruction);
								insCount++;
							}
							else {
								throw CompileTimeErrorException(linecount);
							}
						}
					}
					// If instruction is not found, then it should be in the next line
				}
				// Ioc, it simply has code
				else {
					//std::cout << "This is code" << std::endl;
					if (iss >> tok2) {
						instrucTion instruction(tok1, tok2);
						if (checkInstruction(instruction)) {
							instructionSet_.push_back(instruction);
							insCount++;
						}
						else {
							throw CompileTimeErrorException(linecount);
						}
					}
					else {
						tok2 = "";
						// There are instructions with a single operator
						instrucTion instruction(tok1, tok2);
						if (checkInstruction(instruction)) {
							instructionSet_.push_back(instruction);
							insCount++;
						}
						else {
							throw CompileTimeErrorException(linecount);
						}
					}
				}
			}

			linecount++;
		}
	}

	infile.close();
}

ProgramMemory::~ProgramMemory() {}

instrucTion ProgramMemory::getInstruction(int ip) {
	
	// Return type instrucTion = pair<string, string>
	return instructionSet_[ip];
}

// Specially crafted for jump operations
unsigned int ProgramMemory::getInstrIndexFromLabel(const std::string& label) {
	
	try {
		// Throws out_of_range exception if failed
		return labels_.at(label);
	} catch (const std::out_of_range& e) {
		std::string errorString("No se encuentra la etiqueta: ");
		errorString += label;
		throw std::out_of_range(errorString.c_str());
	}
}

std::vector<std::string> ProgramMemory::getValidInstructions() {
	return validInstructions;
}

unsigned int ProgramMemory::size() {
	return instructionSet_.size();
}

std::ostream& ProgramMemory::toString(std::ostream& os) {
	
	std::cout << "----------------------------------------------------------------------\n"
		<< "			PROGRAM MEMORY\n"
		<< "	- Instruction Set:\n"
		<< "----------------------------------------------------------------------"
		<< std::endl;

	for (unsigned int i = 0; i < instructionSet_.size(); i++) {
		std::cout << i << ": " <<
			instructionSet_[i].first << " "
			<< instructionSet_[i].second << std::endl;
	}

	std::cout << "----------------------------------------------------------------------\n" <<
		"	- Labels: " << std::endl;
	
	for (auto it = labels_.begin(); it != labels_.end(); ++it) {
		std::cout << it->first << ": " << it->second << std::endl;
	}

	std::cout << "----------------------------------------------------------------------\n";

	return os;
}

//---------------------------PRIVATE MEMBERS-------------------------------

bool ProgramMemory::checkInstruction(instrucTion& instruction) {
	
	//Transform to uppercase
	std::transform(instruction.first.begin(),
		instruction.first.end(), instruction.first.begin(), ::toupper);
	std::vector<std::string>::const_iterator it;

	it = std::find(validInstructions.begin(), validInstructions.end(), instruction.first);
	if (it != validInstructions.end()) {
		//std::cout << *it << std::endl;
		return true;
	}

	return false;
}

const std::vector<std::string> ProgramMemory::validInstructions = 
	{"LOAD", "STORE", "ADD", "SUB", "MUL", "DIV", "READ", "WRITE", "JUMP", "JZERO", "JGTZ" ,"HALT"};