#include "Monitor.h"
#include <ostream>

// Тут функции, выполняемые непосредственно через монитор команд
namespace commands_ns {

	//Временная заглушка, которая выводит информацию о системе и список файлов
	void stub(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	//Функция для выхода из прогаммы, которую, нужно вписать в ТЗ.
	void Exit(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void List(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void Lista(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void DiskInfo(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void MkFile(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void DelFile(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void RenFile(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void AddToFile(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void ChType(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void Format(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void ChFsInfo(FileSystem *fs, int argc, char *argv[], std::ostream& out);

	void Cmprs(FileSystem *fs, int argc, char *argv[], std::ostream& out);
}