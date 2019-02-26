/*
	Implementation of methods from personlized exceptions
	Mainly what() for the different classes
*/

#include "../lib/PersExceptions.h"

StreamVectorOOBoundsException::StreamVectorOOBoundsException(const std::string & module)
	: mod_(module) {}

std::string StreamVectorOOBoundsException::mywhat() const noexcept {
	
	std::string
		err("----------------------------------------------------------------------\n");
	err += "	- Se ha sobrepasado la capacidad de una cinta de E / S\n";
	err += "	- En modulo: " + mod_;
	err += "----------------------------------------------------------------------\n";

	return err;
}

// ----------- ---------- --------------- ---------- ---------

BadOperatorException::BadOperatorException(const std::string & op) : op_(op) {}

std::string BadOperatorException::mywhat() const noexcept {
	
	std::string err("----------------------------------------------------------------------\n");
	err += "	- Se ha detectado un operador incorrecto: ";
	err += op_;
	err += "\n\n----------------------------------------------------------------------\n";

	return err;
}

// ----------- ------------- ------------- ---------- --------

CompileTimeErrorException::CompileTimeErrorException(int line) : errline_(line) {}

std::string CompileTimeErrorException::mywhat() const noexcept {

	std::string err("----------------------------------------------------------------------\n");
	err += "	- En la linea:"  + std::to_string(errline_);
	err += "	- Error en tiempo de compilacion del programa RAM.\n";
	err += "\n\n----------------------------------------------------------------------\n";

	return err;
}

// ---------- ----------- ------------ ----------- ----------

ExecTimeErrorException::ExecTimeErrorException(int line) : errline_(line) {}

std::string ExecTimeErrorException::mywhat() const noexcept {
	
	std::string err("\n\n----------------------------------------------------------------------\n");
	err += "	- Error en tiempo de ejecucion del programa RAM.\n"
		   "	- Instruccion apuntada por IP en: " + std::to_string(errline_);
	err += "\n\n----------------------------------------------------------------------\n";

	return err;
}

HaltException::HaltException(int haltline) : haltline_(haltline) {}

std::string HaltException::mywhat() const noexcept {
	
	std::string err("\n----------------------------------------------------------------------\n");
	err += "	- Programa terminado satisfactoriamente(HALT).\n"
		   "	- Instruccion en linea: " + std::to_string(haltline_);
	err += "\n----------------------------------------------------------------------\n";
	
	return err;
}
