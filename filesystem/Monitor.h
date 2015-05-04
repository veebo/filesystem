#include <ostream>
#include <vector>
#include <map>
#include "FileSystem.h"
#include "InputOutput.h"

// Надо для сравнения строк при разборе комманд
struct cmp_str {
	bool operator()(char const *a, char const *b) {
		return std::strcmp(a, b) < 0;
	}
};

class Monitor
{

public:
	Monitor(InputOutput* io);
	~Monitor();
	void CreateFileSystem(char* file_name, char* tom_name, char* owner_name, size_t max_size); //Создание файловой системы
	void Execute(std::vector<char*>* argv); //Выполнение комманды

protected:
	InputOutput* io; //Указатель на экземпляр ввода/вывода
	FileSystem* fs;  //Указатель на экземпляр файловой системы, с которой работает
	std::map<char*, void(*) (FileSystem *fs, int argc, char *argv[], std::ostream& out), cmp_str> commands; // "Название комменды":"соответствующая функция"
	void InitCommands(); //Инициализация функций комманд(т.е. функций, описанных в commands_ns.h)
					     //Тут регистрируется каждая команда монитора

};
