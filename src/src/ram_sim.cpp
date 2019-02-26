/*
	Main file for this RAM simulator project.

*/
#include <iostream>
#include <stdlib.h>

#include "../lib/CPU.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (argc != 5) {
		cerr << "Argumentos insuficientes: " << endl
			<< "Esperados: './ram_sim ram_program.ram input_tape.in output_tape.out debug'" 
			<< endl
			<< "Recibidos: " << argc << endl;
		system("PAUSE");
		return -1;
	}

    int debug;
	string ram_prog, input_f, output_f;
	ram_prog = argv[1];
	input_f = argv[2];
	output_f = argv[3];

	try {
		debug = stoi(argv[4]);		// Could throw an exception

		cout << "----------------------------------------------------------------------\n"
			<< "----------------------------------------------------------------------\n"
			<< "					RAMS: RANDOM ACCESS MACHINE\n"
			<< "							  SIMULATOR\n"
			<< "----------------------------------------------------------------------\n\n" 
			<< endl;
		
		CPU cpu(ram_prog, input_f, output_f, debug);
		cpu.exec();
	}
	catch(const PersException& e){
		// These are handled slightly differently
		cerr << e.mywhat() << endl;
	}
	catch (const std::exception& e){
		cerr << "----------------------------------------------------------------------\n"
			 << "Standard exception_____\n"
			 << e.what()
			<< "\n----------------------------------------------------------------------\n" 
			<< endl;
	}

	system("PAUSE");
	return 0;
}