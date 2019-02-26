#include "../lib/DataMemory.h"

DataMemory::DataMemory() {
	
	regs_[0] = 0;						// Initialization of regs_[0], or accumulator
}


DataMemory::~DataMemory() {}

void DataMemory::write(int index, int value) {
	
	regs_[index] = value;				// Writing does not cause any trouble
}

int DataMemory::read(int index) {
	
	int readInt;
	try {
		readInt = regs_.at(index);	// The method at(index) throws out_of_range when regs_[index] 
										// is not defined
	}
	catch(std::out_of_range& e){
		regs_[index] = 0;
		readInt = 0;
	}

	return readInt;
}

std::ostream & DataMemory::toString(std::ostream &os) {
	
	os << "	- DATA MEMORY:\n\n";
	for (register_iterator it = regs_.begin(); it != regs_.end(); ++it) {
		os << "	|   R[" << it->first << "] = " << it->second << "  |\n";
	}
	os << std::endl;

	return os;
}
