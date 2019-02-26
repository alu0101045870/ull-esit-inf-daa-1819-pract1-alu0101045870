/*
	This header file contains all of the personalized exceptions that have been 
	created for the RAM simulator

	All of them inherit from std::exception
*/

#pragma once

#include <string>
#include <exception>

/**
 *	Class PersException, thought to be a step forward in the inheritance tree so my 
 *  exceptions pend only on this class
 */
class PersException : public std::exception {
public:
	virtual std::string mywhat() const noexcept = 0;	//Implementation of what from std::exception
};


/**
 *  Exception to be thrown when RAM input or output units read out of bounds
 *
 */
class StreamVectorOOBoundsException : public PersException {
private:
	std::string mod_;		//Module that caused the exception
public:
	StreamVectorOOBoundsException(const std::string& module);
	std::string mywhat() const noexcept;		//Implementation of what() from std::exception
};

/**
*	Exception to be thrown when an operator called in an instruction raises an error
*	It may be due to format, meaning within its scope being incorrect or similar causes
*/
class BadOperatorException : public PersException {
private:
	std::string op_;		//Operator that caused the exception
public:
	BadOperatorException(const std::string& op);
	std::string mywhat() const noexcept;
};

/**
*   Exception to be thrown when a "Compile-time" problem when processing .ram file 
*	with DataMemory module
*
*/
class CompileTimeErrorException : public PersException {
private:
	int errline_;
public:
	CompileTimeErrorException(int line);
	std::string mywhat() const noexcept;
};

/**
*	Exception to be thrown when an error occurs during execution of a RAM program
*/
class ExecTimeErrorException : public PersException {
private:
	int errline_;
public:
	ExecTimeErrorException(int line);
	std::string mywhat() const noexcept;
};

class HaltException : public PersException {
private:
	int haltline_;
public:
	HaltException(int haltline);
	std::string mywhat() const noexcept;
};