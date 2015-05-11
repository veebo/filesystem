#include <iostream>
#include <ostream>
#include <string.h>
#include <cstdlib>
#include <cctype>
#include "commands_ns.h"
#include "const.h"

using namespace std;



Monitor::Monitor(InputOutput* _io){
	if (!_io)
		throw "Input/Output must be set";
	io = _io;
	fstream* fp = new fstream(FILE_NAME, std::fstream::in | std::fstream::out);
	
	if (!fp->is_open()) {
		char* str;

		io->WriteLine("File System doesn't exist. Would you like to create it? [y/n]");
		while (!fp->is_open()){
			str = io->ReadLine(" \t")->at(0);
			if (strlen(str) > 0){
				if (tolower(str[0]) == 'y'){
					char *tom, *owner;
					size_t maxsize;
					io->WriteLine("Enter tom's name");
					tom = io->ReadLine(NULL)->at(0);
					io->WriteLine("Enter owners's name");
					owner = io->ReadLine(NULL)->at(0);
					io->WriteLine("Enter maximum size of filesystem (in Kb)");
					while (1) {
						maxsize = atol(io->ReadLine(" \t")->at(0));
						if (maxsize <= 0){
							io->WriteLine("Value is incorrect. Try again");
						} else {
							fp->close();
							CreateFileSystem(FILE_NAME, tom, owner, maxsize);
							InitCommands();
							io->WriteLine("File System created. Enter your command");
							io->WriteLine("");
							return;
						}
					}
				} else if (tolower(str[0]) == 'n'){
					exit(0);
				} else{
					io->GetOutput() << "Bad answer. Type 'y' or 'n'" << endl;
				}
			}
		}
	}
	else {
		fp->close();
		fs = new FileSystem(FILE_NAME);
		InitCommands();
		io->WriteLine("File System loaded. Enter your command");
	}
}

void Monitor::InitCommands(){

	commands["stub"] = commands_ns::stub;
	commands["Exit"] = commands_ns::Exit;
	commands["list"] = commands_ns::List;
	commands["lista"] = commands_ns::Lista;

}

Monitor::~Monitor(){
	delete fs;
	delete io;
}


void Monitor::CreateFileSystem(char* file_name, char* tom_name, char* owner_name, size_t max_size){
	fs = new FileSystem(file_name, tom_name, owner_name, max_size);
}


void Monitor::Execute(std::vector<char*>* argv_vector){
	if (!fs)
		throw "File System doesn't exist";
	if (argv_vector == NULL || argv_vector->size() < 1)
		throw "Missing command";

	char* command_name = argv_vector->at(0);
	int argc = argv_vector->size() - 1;
	char** argv = new char*[argc];

	if (commands.find(command_name) != commands.end()){
		for (int i = 0; i < argc; ++i){
			argv[i] = (argv_vector->at(i+1));
		}
		
		commands[command_name](fs, argc, argv, io->GetOutput());
		io->WriteLine("Command executed.");
	} else {
		char* mes = new char[strlen(command_name) + 22];
		sprintf_s(mes, strlen(command_name) + 23, "Command not found: '%s'", command_name);
		io->WriteLine(mes);
	}
}

