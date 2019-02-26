#include "../lib/InputUnit.h"

InputUnit::InputUnit(const std::string& filename) {
	
	std::ifstream infile;
	infile.exceptions(std::ifstream::badbit);	//throw failure automatically when error occurs
	infile.open(filename);

	if (!infile.good()) {
		//Error handling
		std::string errorString("Error en la apertura del archivo: ");
		errorString += filename;
		throw std::ifstream::failure(errorString.c_str());
	}
	else {
		int input;
		//Read numbers from file
		while (infile >> input) { 
			inputStream_.push_back(input);
		}
		// Init position for reading head
		readingHead_ = 0;
		// Close file
		infile.close();
	}
}

InputUnit::~InputUnit(){}

int InputUnit::read() {
	
	int readNum = -1;

	if (readingHead_ >= inputStream_.size()) {
		//throw pers. exception
		throw StreamVectorOOBoundsException("Input Unit");
	}
	else {
		readNum = inputStream_[readingHead_++];
	}
	return readNum;
}

void InputUnit::resetHead() {
	readingHead_ = 0;
}


std::ostream& InputUnit::toString(std::ostream& os) const {
	
	os << "----------------------------------------------------------------------\n"
		<< "	- INPUT UNIT: " 
		<< "[ ";
	for (unsigned int i = 0; i < inputStream_.size(); i++) {
		if (i < inputStream_.size() - 1) {
			os << inputStream_[i] << ", ";
		}
		else {
			os << inputStream_[i] << " ";
		}
	}
	os << "]\n\n" 
	   << "----------------------------------------------------------------------\n";
	return os;
}

int InputUnit::nexToRead() {

	return (readingHead_ >= inputStream_.size()) ? 
		INT_MAX : inputStream_[readingHead_];
}
