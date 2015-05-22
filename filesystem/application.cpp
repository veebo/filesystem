#include <iostream>
#include "Monitor.h"
#include <conio.h>
#include <locale>


int main(){
	setlocale(LC_ALL,"Russian"); 
	InputOutput* io = new InputOutput(std::cin, std::cout);
	Monitor* monitor = new Monitor(io);
	std::vector<char*>* input;

	while (1){
		try{
			io->Invite();
			input = io->ReadLine(" \t");
			monitor->Execute(input);
		}
		catch (char* e){
			io->WriteLine(e);
		}
	}
	delete monitor;

	return 0;
}
