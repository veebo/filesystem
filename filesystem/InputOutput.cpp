#include "commands_ns.h"
#include <vector>

using namespace std;

InputOutput::InputOutput(istream& _in, ostream& _out): in(_in), out(_out){}


InputOutput::~InputOutput(){}


istream& InputOutput::GetInput(){
	return  in;
}


ostream& InputOutput::GetOutput(){
	return  out;
}

std::vector<char*>* InputOutput::ReadLine(char* separators){
	char input[255];
	vector<char*>* argv = new vector<char*>;

	while (argv->empty()){
		cin.getline(input, 255, '\n');
		Parse(input, separators, argv);
	}
	return argv;
}

void InputOutput::WriteLine(char *line){
	out << line << endl;
}

void InputOutput::Parse(char* args, char* separators, vector<char*>* argv){
	char *p, *next, *temp_val;
	char buffer[255];
	if (strlen(args) > 255){
		throw "Too long input string";
	}
	strcpy_s(buffer, args);
	if (!separators){
		char* end = args + strlen(args) - 1;
		p = args;
		while (p < end && *p == ' ')
			++p;
		while (end >= p && *end == ' ')
			--end;
		if (end < p)
			return;
		temp_val = new char[end - p + 2];
		strcpy_s(temp_val, end - p + 2, p);
		argv->push_back(temp_val);
		return;

	}
	else {
		for (p = strtok_s(buffer, separators, &next); p != NULL; p = strtok_s(NULL, separators, &next)){
			temp_val = new char[strlen(p) + 1];
			strcpy_s(temp_val, strlen(p) + 1, p);
			argv->push_back(temp_val);
		}
	}
}
