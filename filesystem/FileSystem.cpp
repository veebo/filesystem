#include "commands_ns.h"
#include "const.h"
#include <iostream>

using namespace std;


FileSystem::FileSystem(char* file_name) {
	fileName = file_name;
	fp = new fstream(file_name, std::fstream::in | std::fstream::out);
	if (!fp->is_open())
		throw "������ ��� �������� ����� fs.txt";
	first_file = -1;

	validate();
	GetTomName();
	GetOwner();
	GetMaxSize();
	GetFilesCount();
	get_first_file();

	fp->seekp(0, fp->beg);
}


FileSystem::FileSystem(char* file_name,  char* _tomName, char* _owner, size_t _maxSize){
	fileName = file_name;
	fp = new fstream(file_name, std::fstream::in | std::fstream::out | std::fstream::trunc);
	first_file = -1;

	validate();
	SetTomName(_tomName);
	SetOwner(_owner);
	SetMaxSize(_maxSize);
	set_filesCount(0);
	set_first_file(-1);
	write_line(fp, NULL, 0);

	fp->flush();
	fp->seekp(0, fp->beg);
}


FileSystem::~FileSystem(){
	fp->seekp(0, fp->beg);
	fp->close();
}


void FileSystem::Compress(){
	//!!!
}


void FileSystem::CreateFile(FileDescriptor* fd){

	char* type;
	bool buzy = true;
	int index = 0;
	size_t pos;
	size_t new_fd_pos;

	GetFilesCount();
	if (filesCount == MAX_FILES_COUNT)
		throw "��������� ������������ ����������� ������";

	FileIterator* fi = GetIterator();
	int N = GetFilesCount();
	if (N != 0)
	{
		while (fi->HasNext())
		{
			fi->Next();
			FileDescriptor* fdtmp = fi->GetFileDescriptor();
			
			if (strcmpi(fdtmp->GetName(), fd->GetName()) == 0 
				&& strcmpi(fdtmp->GetType(), fd->GetType()) == 0)
				throw "���� � ��������� ������ ��� ����������";
		}
	}

	while (buzy == 1)
	{
		fp->seekg(META_END + (index++)*LINES_PER_FD*LINE_SIZE, fp->beg);
		if (index > filesCount) 
		{
			break;
		}
		else
		{
			type = read_line(fp);
			if (strcmp(type, R_FREE) == 0)
			{
				buzy = false;
			}
		}
	}

	new_fd_pos = META_END + (index - 1)*LINES_PER_FD*LINE_SIZE;

	fp->seekp(new_fd_pos, fp->beg);

	write_line(fp, R_CONST, 6);
	write_line(fp, fd->GetName(), strlen(fd->GetName()));
	write_line(fp, fd->GetType(), strlen(fd->GetType()));
	write_line(fp, fd->GetSize());
	write_line(fp, fd->GetFormatDate(), 10);
	write_line(fp, (size_t)fd->GetOffset());
	write_line(fp, -1);
	write_line(fp, NULL, 0);

	set_filesCount(++filesCount);
	fp->seekp(0, fp->beg);

	get_first_file();
	if (first_file < 0){
		set_first_file(0);
	}
	else {
		FileIterator *iter = GetIterator();
		while (iter->HasNext()){
			iter->Next();
		}
		iter->set_next(index - 1);
	}
	
}


FileIterator* FileSystem::GetIterator(){
	fstream* new_fp = new fstream(fileName, std::fstream::in | std::fstream::out);
	return new FileIterator(this, new_fp);
}

char* FileSystem::GetTomName(){
	fp->seekp(0, fp->beg);
	tomName = read_line(fp);
	return  tomName;
}


char* FileSystem::GetOwner(){
	fp->seekp(LINE_SIZE, fp->beg);
	owner = read_line(fp);
	return  owner;
}


size_t FileSystem::GetMaxSize(){
	fp->seekp(2 * LINE_SIZE, fp->beg);
	maxSize = atol(read_line(fp));
	return  maxSize;
}

size_t FileSystem::GetFilesCount(){
	fp->seekp(3 * LINE_SIZE, fp->beg);
	filesCount = atol(read_line(fp));
	if (filesCount > MAX_FILES_COUNT)
		throw "��������� ������������ ����������� ������";
	return filesCount;
}

size_t FileSystem::get_first_file(){
	fp->seekp(4 * LINE_SIZE, fp->beg);
	first_file = atol(read_line(fp));
	return  first_file;
}

void FileSystem::SetTomName(char* _tomName){
	tomName = _tomName;
	fp->seekp(0, fp->beg);
	write_line(fp, tomName, strlen(tomName));
}

void FileSystem::SetOwner(char* _owner){
	owner = _owner;
	fp->seekp(LINE_SIZE, fp->beg);
	write_line(fp, owner, strlen(owner));
}

void FileSystem::SetMaxSize(size_t _maxSize){
	maxSize = _maxSize;
	fp->seekp(2 * LINE_SIZE, fp->beg);
	write_line(fp, maxSize);
}

void FileSystem::set_filesCount(size_t _filesCount){
	filesCount = _filesCount;
	fp->seekp(3 * LINE_SIZE, fp->beg);
	write_line(fp, filesCount);
}

void FileSystem::set_first_file(int _first_file){
	first_file = _first_file;
	fp->seekp(4 * LINE_SIZE, fp->beg);
	write_line(fp, first_file);
}

void FileSystem::write_line(fstream* fp, char *data, size_t len){
	char* spaces;
	char newline = '\n';
	size_t d = data ? LINE_SIZE - len - 2 : LINE_SIZE - 2;

	spaces = new char[d+1];
	for (size_t i = 0; i < d; ++i){
		spaces[i] = 0x20;
	}

	if (data)
		fp->write(data, len);
	fp->write(spaces, d);
	fp->write(&newline, 1);
}

void FileSystem::write_line(fstream* fp, size_t data){
	char buf[LINE_SIZE - 2];
	sprintf_s(buf, "%Iu", data);
	write_line(fp, buf, strlen(buf));
}

void FileSystem::write_line(fstream* fp, int data){
	char buf[LINE_SIZE - 2];
	sprintf_s(buf, "%d", data);
	write_line(fp, buf, strlen(buf));
}

char* FileSystem::read_line(fstream* fp){
	char *end;
	char* beg = new char[LINE_SIZE - 1];
	char* buf;
	fp->getline(beg, LINE_SIZE - 1, '\n');
	end = beg + LINE_SIZE - 3;

	while (end >= beg && *end == ' ')
		--end;
	if (end < beg)
		return "";
	*(end + 1) = '\0';
	buf = new char[end - beg + 2];
	strcpy_s(buf, end - beg + 2, beg);
	return buf;
}

bool FileSystem::fssize(char *maxfssize)
{
	bool flag=false;
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
	return flag;
}

bool FileSystem::names_types(char *str){
	bool flag = false;
	if (strlen(str) > 20 || strlen(str) <= 0){
		flag =true;
	}else
	{
		for (int i = 0;i<strlen(str);++i)
		{
			int k = (int)str[i];
			if (k<48 || k>122 ||(k>57 && k<65)||(k>90 && k<97)){
				flag = true;
				break;
			}
		}
	}
	return flag;
}

void FileSystem::validate(){
	fp->seekg(0, fp->end);
	size_t pos = fp->tellg();
	if (pos % LINE_SIZE != 0)
		throw "������������ ���� fs.txt.";
	fp->seekg(0, fp->beg);
	fp->seekp(0, fp->beg);
	return;
	
}
