#include <ctime>

class FileDescriptor
{

public:
	FileDescriptor(); 
	FileDescriptor(char* _name, char* _type, size_t _size, char* date_repr, size_t _offset);
	~FileDescriptor();

	time_t GetDate(); // Получение даты в формате time_t
	char * GetFormatDate(); //Получение даты в текстовом формате: YYYY DD MM
	size_t GetOffset();
	char * GetName();
	size_t GetSize();
	char * GetType();

	void SetDate(time_t _date); //Задание даты в текстовом формате: YYYY DD MM
	void SetFormatDate(char* repr); //Задание даты в текстовом формате: YYYY DD MM
	void SetOffset(size_t offset); 
	void SetName(char* _name);
	void SetSize(size_t _size);
	void SetType(char* _type);

private:
	time_t date; //Дата 
	char* name; //Имя
	size_t size; // Размер (в блоках)
	char* type; // Тип файла(!НЕ тип записи)
	size_t offset; //Смещение реального файла
};
