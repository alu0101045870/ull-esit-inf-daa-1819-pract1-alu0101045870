#include "../lib/OutputUnit.h"

OutputUnit::OutputUnit(const std::string& filename) {

	// Open file for output and truncate so the content gets discarded each time
	outfile_.open(filename, std::ofstream::out | std::ofstream::trunc);
	outfile_.exceptions(std::ofstream::badbit);	

	if(!outfile_.good()) {
		// The file did not open properly
		std::string errorString("Error en la apertura del archivo: ");
		errorString += filename;
		throw std::ofstream::failure(errorString.c_str());
	}
}


OutputUnit::~OutputUnit() {

	// Write output in file and close
	for (unsigned int i = 0; i < outputStream_.size(); i++){
		outfile_ << outputStream_[i] << " ";
	}
	outfile_.close();
}

void OutputUnit::write(int arg) {
	outputStream_.push_back(arg);
}

std::ostream & OutputUnit::toString(std::ostream & os) {
	
	os << "\n	- OUTPUT UNIT: " << "[ ";
	for (unsigned int i = 0; i < outputStream_.size(); i++) {
		if (i < outputStream_.size() - 1) {
			os << outputStream_[i] << ", ";
		}
		else {
			os << outputStream_[i] << " ";
		}
	}
	os << "]\n\n";

	return os;
}
