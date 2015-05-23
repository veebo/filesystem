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
		throw "Input/Output не задан";
	io = _io;
	fstream* fp = new fstream(FILE_NAME, std::fstream::in | std::fstream::out);
	
	if (!fp->is_open()) {
		while (!fp->is_open()){
					char *tom, *owner,*maxfssize;
					size_t maxsize;
					bool flag;
					io->WriteLine("Необходимо создать файловую систему.");
					io->WriteLine("Введите название тома и нажмите клавишу ENTER");
					//Должна быть циклическая проверка тома
					do{
						flag=false;
						tom = io->ReadLine(NULL)->at(0);
						if (strlen(tom) > 20 || strlen(tom) <= 0){
							flag =true;
						}else
						{
							for (int i = 0;i<strlen(tom);++i)
							{
								int k = (int)tom[i];
								if (k<48 || k>122 ||(k>57 && k<65)||(k>90 && k<97)){
									flag = true;
									break;
								}
							}
						}
						if (flag) io->WriteLine("Введены некорректные данные");
					}while (flag);
					io->WriteLine("Введите имя владельца и нажмите клавишу ENTER");
					//Должна быть циклическая проверка владельца
					do{
						flag=false;
						owner = io->ReadLine(NULL)->at(0);
						if (strlen(owner) > 20 || strlen(owner) <= 0){
							flag =true;
						}else
						{
							for (int i = 0;i<strlen(owner);++i)
							{
								int k = (int)owner[i];
								if (k<48 || k>122 ||(k>57 && k<65)||(k>90 && k<97)){
									flag = true;
									break;
								}
							}
						}
						if (flag) io->WriteLine("Введены некорректные данные");
					}while (flag);
					io->WriteLine("Введите максимальный объем и нажмите клавишу ENTER");
					
					do{
						flag=false;
						maxfssize = io->ReadLine(NULL)->at(0);
						for (int i=0;i<strlen(maxfssize);++i)
						{
							if ((unsigned int)(maxfssize[i])<48||(unsigned int)(maxfssize[i])>57)
							{
								flag=true;
								break;
							}
						}
						if (!flag){
							if ((maxfssize[0]=='-')||(strlen(maxfssize) > 10))
								{
									flag=true;
								}
								else if (strlen(maxfssize) == 10){
										//2147483647
										char MAX_FS_SIZE[]={'2','1','4','7','4','8','3','6','4','7'};
										for (int i=0;i<10;++i)
											{
												if (maxfssize[i]>MAX_FS_SIZE[i]){
												flag=true;
												break;
												}else if (maxfssize[i]<MAX_FS_SIZE[i]) break;
											}
								}else if (atol(maxfssize)==0) flag=true;
						}
						if (flag) io->WriteLine("Введены некорректные данные"); else maxsize=atol(maxfssize);
					}while (flag);
					fp->close();
					CreateFileSystem(FILE_NAME, tom, owner, maxsize);
					InitCommands();
					io->WriteLine("Файловая система создана");
					io->WriteLine("");
					return;
		}
	}
	else {
		fp->close();
		fs = new FileSystem(FILE_NAME);
		InitCommands();
		io->WriteLine("Файловая система загружена");
	}
}

void Monitor::InitCommands(){
	
	commands["exit"] = commands_ns::Exit;
	commands["list"] = commands_ns::List;
	commands["lista"] = commands_ns::Lista;
	commands["diskinfo"] = commands_ns::DiskInfo;
	commands["cmprs"] = commands_ns::Cmprs;
	commands["chfsinfo"] = commands_ns::ChFsInfo;
	commands["format"] = commands_ns::Format;
	commands["renfile"] = commands_ns::RenFile;
	commands["chtype"] = commands_ns::ChType;
	commands["mkfile"] = commands_ns::MkFile;
	commands["delfile"] = commands_ns::DelFile;
	commands["delfile"] = commands_ns::DelFile;
	commands["addtofile"] = commands_ns::AddToFile;
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
		throw "Файловая система не существует";
	if (argv_vector == NULL || argv_vector->size() < 1)
		throw "Ожидается команда";

	char* command_name = argv_vector->at(0);
	int argc = argv_vector->size() - 1;
	char** argv = new char*[argc];

	int len = strlen(command_name);
	for (int i = 0; i < len; ++i){
		command_name[i] = tolower(command_name[i]);
	}

	if (commands.find(command_name) != commands.end()){
		for (int i = 0; i < argc; ++i){
			argv[i] = (argv_vector->at(i+1));
		}
		
		commands[command_name](fs, argc, argv, io->GetOutput());
		//io->WriteLine("Command executed.");
	} else {
		//char* mes = new char[strlen(command_name) + 22];
		//sprintf_s(mes, strlen(command_name) + 23,  : '%s'", command_name);
		//io->WriteLine(mes);
		io->WriteLine("Не удалось распознать команду");
	}
}

