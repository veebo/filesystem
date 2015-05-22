#include "FileIterator.h"

class FileSystem
{
	friend class FileIterator;
public:
	FileSystem(char* file_name);
	FileSystem(char* file_name, char* _tomName, char* _owner, size_t _maxSize);
	~FileSystem();
	void Compress(); //Ñæàòèå ôàéëîâîé ñèñòåìû
	void CreateFile(FileDescriptor* fd); //Ñîçäàíèå ôàéëà
	FileIterator* GetIterator(); //Ïîëó÷åíèå ýêçåìïëÿðà èòåðàòîðà
	size_t GetMaxSize();
	char *GetOwner();
	char *GetTomName();
	size_t GetFilesCount();
	void SetMaxSize(size_t _maxSize);
	void SetOwner(char* _owner);
	void SetTomName(char* _tomName);
	bool fssize(char *);
	bool names_types(char *);
	void set_first_file(int _first_file);


private:
	char* fileName; //Èìÿ ôàéëà, â êîòîðîì õðàíèòñÿ èíôà (fs.txt ïî-óìîë÷àíèþ)
	char* tomName; //Íàçâàíèå ðàçäåëà
	char* owner; //Èìÿ âëàäåëüöà
	size_t maxSize; //Ìàêñèìàëüíûé ðàçìåð ñèñòåìû
	size_t filesCount; //Êîëëè÷åñòâî ôàéëîâ
	int first_file; //Èíäåêñ ïåðâîãî ôàéëà
	std::fstream* fp; //Óêàçàòåëü íà ôàéëîâûé ïîòîê äëÿ fs.txt
	size_t get_first_file();
	void set_filesCount(size_t _filesCount);

	// Ñòàòè÷åñêèå ôóíêöèè äëÿ çàïèñè-÷òåíèÿ ñòðîê. Èñïîëüçóþòñÿ ýòèì êëàññîì è FileIterator.
	static void write_line(std::fstream * fp, char *data, size_t len); 
	static void write_line(std::fstream * fp, size_t data);
	static void write_line(std::fstream * fp, int data);
	static char* read_line(std::fstream * fp);
};
