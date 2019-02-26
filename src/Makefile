OBJS = ram_sim.o ProgramMemory.o PersExceptions.o OutputUnit.o InputUnit.o DataMemory.o CPU.o
BINARY = ram_sim
CFLAGS = -std=c++11 -o
DIR = src/

all: ram_sim 

#This rule compiles the main program
ram_sim: $(OBJS)
	g++ $(CFLAGS) $(BINARY) $(OBJS) 

ram_sim.o: src/ram_sim.cpp
	g++ -c src/ram_sim.cpp


ProgramMemory.o: src/ProgramMemory.cpp
	g++ -c src/ProgramMemory.cpp

DataMemory.o: src/DataMemory.cpp
	g++ -c src/DataMemory.cpp

PersExceptions.o: src/PersExceptions.cpp
	g++ -std=c++11 -c src/PersExceptions.cpp

OutputUnit.o: src/OutputUnit.cpp
	g++ -std=c++11 -c src/OutputUnit.cpp

InputUnit.o: src/InputUnit.cpp
	g++ -c src/InputUnit.cpp

CPU.o: src/CPU.cpp
	g++ -c src/CPU.cpp

clean:
	rm -f $(BINARY) *.o
