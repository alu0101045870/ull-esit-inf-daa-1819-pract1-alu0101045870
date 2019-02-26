/*
	File containing definition of class OutputUnit, component whose purpose is 
	writing the output stream to a given file
*/
#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

#include "PersExceptions.h"

/*

*/
class OutputUnit {
private:
	std::ofstream outfile_;				// File stream needs to be open till the 			
	std::vector<int> outputStream_;		// Contains data to be stored on file at the end
public:
	//Constructor & Destructor
	OutputUnit(const std::string& filename);	
	~OutputUnit();						// Data is efectively passed to output file

	// Write in outputStream_
	void write(int arg);

	// Debug mode
	std::ostream& toString(std::ostream& os);
};

