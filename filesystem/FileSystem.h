#include "FileIterator.h"

class FileSystem
{
	friend class FileIterator;
public:
	FileSystem(char* file_name);
	FileSystem(char* file_name, char* _tomName, char* _owner, size_t _maxSize);
	~FileSystem();
	void Compress(); //Сжатие файловой системы
	void CreateFile(FileDescriptor* fd); //Создание файла
	FileIterator* GetIterator(); //Получение экземпляра итератора
	size_t GetMaxSize();
	char *GetOwner();
	char *GetTomName();
	size_t GetFilesCount();
	void SetMaxSize(size_t _maxSize);
	void SetOwner(char* _owner);
	void SetTomName(char* _tomName);


private:
	char* fileName; //Имя файла, в котором хранится инфа (fs.txt по-умолчанию)
	char* tomName; //Название раздела
	char* owner; //Имя владельца
	size_t maxSize; //Максимальный размер системы
	size_t filesCount; //Колличество файлов
	int first_file; //Индекс первого файла
	std::fstream* fp; //Указатель на файловый поток для fs.txt
	void set_first_file(int _first_file);
	size_t get_first_file();
	void set_filesCount(size_t _filesCount);

	// Статические функции для записи-чтения строк. Используются этим классом и FileIterator.
	static void write_line(std::fstream * fp, char *data, size_t len); 
	static void write_line(std::fstream * fp, size_t data);
	static void write_line(std::fstream * fp, int data);
	static char* read_line(std::fstream * fp);
};
