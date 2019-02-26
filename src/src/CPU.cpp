#include "../lib/CPU.h"

void CPU::showInitDebugInfo() {
	// PROGRAM MEMORY
	// INPUT UNIT
	std::cout << "DEBUG MODE: ON" << std::endl;
	programMem_.toString(std::cout);
	std::cout << std::endl;
	inputUnit_.toString(std::cout);
}

void CPU::iterDebugInfo(const instrucTion& ins) {
	/*
		Iter::
			- IP
			- Program Memory -> Instruction pointed to
			- DataMemory toString
			- InputUnit -> Number to be read next
			- OutputUnit -> Current content
	*/
	std::cout << "----------------------------------------------------------------------\n"
		<< "					  INFORMACION DE DEPURACION: \n"
		<< "	- IP: " << IP
		<< "\n	- Siguiente instruccion a ejecutar: " << ins.first << " " << ins.second
		<< "\n";
		std::cout << "	- Siguiente valor a leer en la cinta de entrada: ";
		if (inputUnit_.nexToRead() == INT_MAX)	std::cout << "OUT_OF_BOUNDS\n";
		else									std::cout << inputUnit_.nexToRead() << "\n";
	
		dataMem_.toString(std::cout);
		outputUnit_.toString(std::cout);

		system("PAUSE");
}

CPU::CPU(const std::string& ram_filename, const std::string& input_filename,
	const std::string& output_filename, int debug) 
	: inputUnit_(input_filename), 
	outputUnit_(output_filename), 
	programMem_(ram_filename), 
	dataMem_(),
	debug_(debug),
	IP(0) {

	insCount = 0;		//Initialized as 0
	
	// THIS IS MY INSTRUCTION SET
	// {"LOAD", "STORE", "ADD", "SUB", "MUL", "DIV", "READ", "WRITE", "JUMP", "JZERO", "JGTZ" ,"HALT"};
	RAMinstructions_["LOAD"] = new LoadRamF();
	RAMinstructions_["STORE"] = new StoreRamF();
	RAMinstructions_["ADD"] = new AddRamF();
	RAMinstructions_["SUB"] = new SubRamF();
	RAMinstructions_["MUL"] = new MulRamF();
	RAMinstructions_["DIV"] = new DivRamF();
	RAMinstructions_["READ"] = new ReadRamF();
	RAMinstructions_["WRITE"] = new WriteRamF();
	RAMinstructions_["JUMP"] = new JumpRamF();
	RAMinstructions_["JZERO"] = new JZeroRamF();
	RAMinstructions_["JGTZ"] = new JGtzRamF();
	RAMinstructions_["HALT"] = new HaltRamF();

	std::cout << "Programa cargado correctamente: " << std::endl;
}

CPU::~CPU() {
	// RELEASE MEMORY FROM POINTERS IN MAGICAL INSTRUCTION MAP
	for (auto it = RAMinstructions_.begin(); it != RAMinstructions_.end(); ++it) {
		delete it->second;
	}

	// OUTPUT FILE IS WRITTEN IN OUTPUT UNIT DESTRUCTOR
}

void CPU::exec() {

	if (debug_) showInitDebugInfo();
	else std::cout << "DEBUG MODE: OFF" << std::endl;

	std::cout << "Iniciando ejecucion de programa..." << std::endl;

	try {
		// 1. Get instruction from programMem_
		// 2. Execute it
		while (IP < programMem_.size()) {	
			instrucTion instruction = programMem_.getInstruction(IP);
			
			if (debug_) iterDebugInfo(instruction);		// DebugMode

			(*RAMinstructions_[instruction.first])(instruction.second, this);
			insCount++;
		}
		std::cout << "	Programa terminado. No hay mas instrucciones por leer." << std::endl
			<< "----------------------------------------------------------------------\n" 
			<< "	- Numero de instrucciones ejecutadas: " << insCount << std::endl;
	}
	catch(BadOperatorException& e) {
		// There has been an error with an operator
		std::cerr << e.mywhat() << std::endl;
		throw ExecTimeErrorException(IP);
	}
	catch(HaltException& e) {
		// A stop signal has been sent
		std::cout << "----------------------------------------------------------------------\n"
			<< "	- Numero de instrucciones ejecutadas: " << ++insCount << std::endl
			<< "	- Codigo de salida: 0\n";
		throw HaltException(IP);
	}
}

void CPU::nextInstr() {
	IP++;
}

void CPU::setIP(unsigned int index) {
	IP = index;
}

// --------------- ARCHITECTURE INSTRUCTIONS -----------------

CPU::RamFunction::RamFunction() {}
CPU::RamFunction::~RamFunction() {}

void CPU::LoadRamF::operator()(const std::string & op, CPU* procUnit) {
	
	// Three possibilities, essentially: const, direct and indirect
	std::regex constant("(=)[[:digit:]]+");			// Equal and then an integer
	std::regex direct("([[:digit:]]+)");			// An integer only
	std::regex indirect("(\\*)[[:digit:]]+");		// * and then integer

	std::string aux_op = op;		// We need to modify the operator
	
	if (std::regex_match(op, constant)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		// Loads into register 0 the constant in real_op
		procUnit -> dataMem_.write(0, real_op);
	}
	else if (std::regex_match(op, direct)) {
		int real_op = stoi(aux_op);

		// Loads into register 0 number in register[real_op]
		int numFromRegister = procUnit->dataMem_.read(real_op);
		procUnit->dataMem_.write(0, numFromRegister);
	}
	else if (std::regex_match(op, indirect)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		// Loads into register 0 number in register pointed from register[real_op]
		int regFromRegister = procUnit->dataMem_.read(real_op);
		int numFromRemRegister = procUnit->dataMem_.read(regFromRegister);
		procUnit->dataMem_.write(0, numFromRemRegister);
	}
	else {
		// Not valid, throw exception
		// CPU manages error at line and stuff
		throw BadOperatorException(op);
	}

	procUnit->nextInstr();			// IP++
}

// -------------------------------------------------------------------

void CPU::StoreRamF::operator()(const std::string & op, CPU * procUnit) {
	
	// STORE only allows direct and indirect
	std::regex direct("([[:digit:]]+)");			// An integer only
	std::regex indirect("(\\*)[[:digit:]]+");		// * and then integer

	std::string aux_op = op;

	if (std::regex_match(op, direct)) {
		int real_op = stoi(aux_op);

		int accRead = procUnit->dataMem_.read(0);
		procUnit->dataMem_.write(real_op, accRead);
	}
	else if (std::regex_match(op, indirect)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int accRead = procUnit->dataMem_.read(0);
		int regRead = procUnit->dataMem_.read(real_op);
		procUnit->dataMem_.write(regRead, accRead);
	}
	else {
		// ERROR. op in not valid
		throw BadOperatorException(op);
	}

	procUnit->nextInstr();
}

//------------------------------------------------------------------

void CPU::AddRamF::operator()(const std::string & op, CPU * procUnit) {
	
	// Three possibilities, essentially: const, direct and indirect
	std::regex constant("(=)[[:digit:]]+");			// Equal and then an integer
	std::regex direct("([[:digit:]]+)");			// An integer only
	std::regex indirect("(\\*)[[:digit:]]+");		// * and then integer

	std::string aux_op = op;

	if (std::regex_match(op, constant)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int result = real_op + (procUnit->dataMem_.read(0));
		procUnit->dataMem_.write(0, result);
	}
	else if (std::regex_match(op, direct)) {
		int real_op = stoi(aux_op);

		int result = (procUnit->dataMem_.read(real_op)) +
			(procUnit->dataMem_.read(0));
		procUnit->dataMem_.write(0, result);
	}
	else if (std::regex_match(op, indirect)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int regPointed = procUnit->dataMem_.read(real_op);
		int result = (procUnit->dataMem_.read(regPointed)) +
			(procUnit->dataMem_.read(0));
		procUnit->dataMem_.write(0, result);
	}
	else {
		// ERROR. op is not valid
		throw BadOperatorException(op);
	}

	procUnit->nextInstr();
}

//-----------------------------------------------------------------------

void CPU::SubRamF::operator()(const std::string & op, CPU * procUnit) {
	
	// Three possibilities, essentially: const, direct and indirect
	std::regex constant("(=)[[:digit:]]+");			// Equal and then an integer
	std::regex direct("([[:digit:]]+)");			// An integer only
	std::regex indirect("(\\*)[[:digit:]]+");		// * and then integer

	std::string aux_op = op;

	if (std::regex_match(op, constant)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int result = (procUnit->dataMem_.read(0)) - real_op;
		procUnit->dataMem_.write(0, result);
	}
	else if (std::regex_match(op, direct)) {
		int real_op = stoi(aux_op);

		int result = (procUnit->dataMem_.read(0)) -
			(procUnit->dataMem_.read(real_op));
		procUnit->dataMem_.write(0, result);
	}
	else if (std::regex_match(op, indirect)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int regPointed = procUnit->dataMem_.read(real_op);
		int result = (procUnit->dataMem_.read(0)) -
			(procUnit->dataMem_.read(regPointed));
		procUnit->dataMem_.write(0, result);
	}
	else {
		// ERROR. op is not valid
		throw BadOperatorException(op);
	}

	procUnit->nextInstr();
}

// --------------------------------------------------------------------

void CPU::MulRamF::operator()(const std::string & op, CPU * procUnit) {

	// Three possibilities, essentially: const, direct and indirect
	std::regex constant("(=)[[:digit:]]+");			// Equal and then an integer
	std::regex direct("([[:digit:]]+)");			// An integer only
	std::regex indirect("(\\*)[[:digit:]]+");		// * and then integer

	std::string aux_op = op;

	if (std::regex_match(op, constant)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int result = (procUnit->dataMem_.read(0)) * real_op;
		procUnit->dataMem_.write(0, result);
	}
	else if (std::regex_match(op, direct)) {
		int real_op = stoi(aux_op);

		int result = (procUnit->dataMem_.read(0)) *
			(procUnit->dataMem_.read(real_op));
		procUnit->dataMem_.write(0, result);
	}
	else if (std::regex_match(op, indirect)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int regPointed = procUnit->dataMem_.read(real_op);
		int result = (procUnit->dataMem_.read(0)) *
			(procUnit->dataMem_.read(regPointed));
		procUnit->dataMem_.write(0, result);
	}
	else {
		// ERROR. op is not valid
		throw BadOperatorException(op);
	}

	procUnit->nextInstr();
}

// ------------------------------------------------------------------

void CPU::DivRamF::operator()(const std::string & op, CPU * procUnit) {

	// Three possibilities, essentially: const, direct and indirect
	std::regex constant("(=)[[:digit:]]+");			// Equal and then an integer
	std::regex direct("([[:digit:]]+)");			// An integer only
	std::regex indirect("(\\*)[[:digit:]]+");		// * and then integer

	std::string aux_op = op;

	if (std::regex_match(op, constant)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int result = (procUnit->dataMem_.read(0)) / real_op;
		procUnit->dataMem_.write(0, result);
	}
	else if (std::regex_match(op, direct)) {
		int real_op = stoi(aux_op);

		int result = (procUnit->dataMem_.read(0)) /
			(procUnit->dataMem_.read(real_op));
		procUnit->dataMem_.write(0, result);
	}
	else if (std::regex_match(op, indirect)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);

		int regPointed = procUnit->dataMem_.read(real_op);
		int result = (procUnit->dataMem_.read(0)) /
			(procUnit->dataMem_.read(regPointed));
		procUnit->dataMem_.write(0, result);
	}
	else {
		// ERROR. op is not valid
		throw BadOperatorException(op);
	}

	procUnit->nextInstr();
}

void CPU::ReadRamF::operator()(const std::string & op, CPU * procUnit) {
	
	// Read operations do not allow constant operator
	std::regex direct("([[:digit:]]+)");			// An integer only
	std::regex indirect("(\\*)[[:digit:]]+");		// * and then integer

	std::string aux_op = op;
	
	// We also need to check if op is zero (not valid)
	if (std::regex_match(op, direct)) {
		int real_op = stoi(aux_op);
		if (real_op == 0) throw BadOperatorException(op);

		int inputVal = procUnit->inputUnit_.read();
		procUnit->dataMem_.write(real_op, inputVal);
	}
	else if (std::regex_match(op, indirect)) {
		aux_op.erase(aux_op.begin());		// We want the reg pointed by op
		int real_op = procUnit->dataMem_.read(stoi(aux_op));
		
		if (real_op == 0) throw BadOperatorException(op);

		int inputVal = procUnit->inputUnit_.read();
		procUnit->dataMem_.write(real_op, inputVal);
	}
	else {
		// ERROR: op format is incorrect
		throw BadOperatorException(op);
	}

	procUnit->nextInstr();
}

// -------------------------------------------------------------------------

void CPU::WriteRamF::operator()(const std::string & op, CPU * procUnit) {

	// These allow every type except WRITE 0
	std::regex constant("(=)[[:digit:]]+");			// Equal and then an integer
	std::regex direct("([[:digit:]]+)");			// An integer only
	std::regex indirect("(\\*)[[:digit:]]+");		// * and then integer

	std::string aux_op = op;

	if (std::regex_match(op, direct)) {
		int real_op = stoi(aux_op);
		if (real_op == 0) throw BadOperatorException(op);

		procUnit->outputUnit_.write(procUnit->dataMem_.read(real_op));
	}
	else if (std::regex_match(op, constant)) {
		aux_op.erase(aux_op.begin());
		int real_op = stoi(aux_op);
		
		procUnit->outputUnit_.write(real_op);
	}
	else if (std::regex_match(op, indirect)) {
		aux_op.erase(aux_op.begin());		// We want the reg pointed by op
		int real_op = procUnit->dataMem_.read(stoi(aux_op));

		if (real_op == 0) throw BadOperatorException(op);

		procUnit->outputUnit_.write(procUnit->dataMem_.read(real_op));
	}
	else {
		// ERROR: op format is incorrect
		throw BadOperatorException(op);
	}

	procUnit->nextInstr();
}

// ------------------------------------------------------------------------

void CPU::JumpRamF::operator()(const std::string & op, CPU * procUnit) {
	
	// We get instruction index at label op
	// This throws an out_of_range exception if label is not found
	unsigned int newIP = procUnit->programMem_.getInstrIndexFromLabel(op);
	procUnit->setIP(newIP);
}

// ------------------------------------------------------------------------

void CPU::JZeroRamF::operator()(const std::string & op, CPU * procUnit) {

	// We get instruction index at label op
	// This throws an out_of_range exception if label is not found
	unsigned int newIP = procUnit->programMem_.getInstrIndexFromLabel(op);
	if (procUnit->dataMem_.read(0) == 0)
		procUnit->setIP(newIP);
	else
		procUnit->nextInstr();
	// Else do nothing (IP++)
}

// ------------------------------------------------------------------------

void CPU::JGtzRamF::operator()(const std::string & op, CPU * procUnit) {

	// We get instruction index at label op
	// This throws an out_of_range exception if label is not found
	unsigned int newIP = procUnit->programMem_.getInstrIndexFromLabel(op);
	if (procUnit->dataMem_.read(0) > 0)
		procUnit->setIP(newIP);
	else
		procUnit->nextInstr();
	// Else do nothing (IP++)
}

void CPU::HaltRamF::operator()(const std::string & op, CPU * procUnit) {

	// op is ""
	throw HaltException(0);			// Line does not matter
									// Exception will be re-thrown
}
