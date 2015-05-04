#include <cstdio>
#include "commands_ns.h"

using namespace std;


FileDescriptor::FileDescriptor(){
	name = "";
	type = "";
	size = -1;
	date = -1;
	offset = -1;
	time(&date);
}


FileDescriptor::FileDescriptor(char* _name, char* _type, size_t _size, char* date_repr, size_t _offset){
	SetName(_name);
	SetType(_type);
	SetSize(_size);
	SetFormatDate(date_repr);
	SetOffset(_offset);
}


FileDescriptor::~FileDescriptor(){
	delete name;
	delete type;
}

time_t FileDescriptor::GetDate(){
	return  date;
}

char * FileDescriptor::GetFormatDate(){
	struct tm *t = new tm();
	char* repr = new char[11];
	localtime_s(t, &date);
	strftime(repr, 11, "%Y %m %d", t);
	return repr;
}


size_t FileDescriptor::GetOffset(){
	return offset;
}


char * FileDescriptor::GetName(){
	return  name;
}


size_t FileDescriptor::GetSize(){
	return  size;
}


char * FileDescriptor::GetType(){
	return type;
}


void FileDescriptor::SetDate(time_t _date){
	date = _date;
}

void FileDescriptor::SetFormatDate(char* repr){
	int year, day, month;
	struct tm* t = new tm();
	sscanf_s(repr, "%d %d %d", &year, &day, &month);
	t = new tm();
	t->tm_year = year - 1900;
	t->tm_mon = day - 1;
	t->tm_mday = month;
	date = mktime(t);
}


void FileDescriptor::SetOffset(size_t _offset){
	offset = _offset;
}


void FileDescriptor::SetName(char* _name){
	name = new char[strlen(_name) + 1];
	strcpy_s(name, strlen(_name) + 1, _name);
}


void FileDescriptor::SetSize(size_t _size){
	size = _size;
}


void FileDescriptor::SetType(char* _type){
	type = new char[strlen(_type) + 1];
	strcpy_s(type, strlen(_type) + 1, _type);
}