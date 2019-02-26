/*

*/
#pragma once

#include "DataMemory.h"
#include "InputUnit.h"
#include "OutputUnit.h"
#include "ProgramMemory.h"

/*

*/
class CPU {
protected:
	// Definition of classes that define instruction behaviour
	// Only available for CPU in protected scope (could be private at this point)
	class RamFunction {
	public:
		RamFunction();
		virtual ~RamFunction();

		virtual void operator()(const std::string& op, CPU* procUnit) = 0;
	};

	/*
		Function implementation for instruction LOAD. Stores "op" in R0
		Allows constant, direct and indirect op
	*/
	class LoadRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction STORE. R0 goes to other register
		Allows direct and indirect op
	*/
	class StoreRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction ADD. R0 += (op)
		Allows constant, direct and indirect op
	*/
	class AddRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction SUB. R0 -= (op)
		Allows constant, direct and indirect op
	*/
	class SubRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction MUL. R0 *= op
		Allows constant, direct and indirect op
	*/
	class MulRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction DIV. R0 /= op
		Allows constant, direct and indirect op
	*/
	class DivRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction READ. 
		Reads from input stream and stores in (op)
		Allows direct and indirect op.
		Raises BadOperatorException when op leads to accumm R0
	*/
	class ReadRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction WRITE. 
		Stores (op) in output stream
		Allows direct and indirect op.
		Raises BadOperatorException when op leads to accumm R0
	*/
	class WriteRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction JUMP.
		Non-conditional jump is performed to label (op)
		Raises an out_of_range exception when label is incorrect
	*/
	class JumpRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction JZERO
		Jumps to label (op) only if R0 == 0
		Raises an out_of_range exception when label is incorrect
	*/
	class JZeroRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction JGTZ
		Jumps to label (op) only if R0 > 0
		Raises an out_of_range exception when label is incorrect
	*/
	class JGtzRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

	/*
		Function implementation for instruction HALT
		Raises exception and stops execution
	*/
	class HaltRamF : public RamFunction {
	public:
		void operator()(const std::string& op, CPU* procUnit);
	};

// ----------------------- CPU class ------------------------------

private:
	InputUnit inputUnit_;
	OutputUnit outputUnit_;
	DataMemory dataMem_;
	ProgramMemory programMem_;
	std::map<std::string, RamFunction*> RAMinstructions_;		//Magic map

	bool debug_;
	unsigned int IP;			// Instruction Pointer
	unsigned int insCount;		// Num of instructions executed (debug)

	void showInitDebugInfo();	// Debug mode ON
	void iterDebugInfo(const instrucTion& ins);	

public:
	CPU(const std::string& ram_filename, const std::string& input_filename,
		const std::string& output_filename, int debug);
	~CPU();

	void exec();			// Main execution thread 
	void nextInstr();		// IP++
	void setIP(unsigned int index);
							
};

