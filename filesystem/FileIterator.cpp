#include "commands_ns.h"
#include "const.h"

using namespace std;


FileIterator::FileIterator(FileSystem* _fs, fstream* _fp){
	fs = _fs;
	fp = _fp;
	next = fs->first_file;

	if (!fp->is_open())
		return;
	if (next < 0)
		return;
	fp->seekg(META_END + next*LINES_PER_FD*LINE_SIZE, fp->beg);
}


FileIterator::~FileIterator(){
	Close();
}


void FileIterator::Close(){
	if (!fp->is_open())
		return;
	fp->seekg(0, fp->beg);
	fp->close();
}


void FileIterator::Delete(){
	//!!!
	//Нужно 1) поставить для текущей записи тип R_FREE, 2) уменьшить
	//колличество файлов в системе на 1 и 3)изменить ссылки(т.е. предыдущий файл
	// должен будет ссылаться на следующий за удаленным). Так же важно предусмотреть
	// ситуацию, если удаляется первый файл в системе(т.е. поле first_file в файловой
	// системе указывает на него). Вариант пунктов 1),2) предоставлен в коде ниже

	//if (!fp->is_open())
	//	throw "Iterator is closed";
	//size_t pos = fp->tellg();
	//FileSystem::write_line(fp, R_FREE, 6); // 1)
	//fp->seekg(pos, fp->beg);
	//fs->set_filesCount(fs->GetFilesCount() - 1); // 2)

}


void FileIterator::Next(){
	if (!fp->is_open())
		throw "Iterator is closed";
	if (next < 0)
		throw "End is reached";
	fp->seekg(META_END + next*LINES_PER_FD*LINE_SIZE, fp->beg);
	read_fd();
}

bool FileIterator::HasNext(){
	if (!fp->is_open())
		throw "Iterator is closed";

	int next;
	if (!current){
		next = fs->get_first_file();
	} else {
		size_t pos = fp->tellg();
		fp->seekg(pos + (LINES_PER_FD - 2)*LINE_SIZE, fp->beg);
		next = atoi(FileSystem::read_line(fp));
		fp->seekg(pos, fp->beg);
	}
	if (next != -1)
		return true;
	return false;
}

void FileIterator::SetFileDescriptor(FileDescriptor* new_fd){
	if (!fp->is_open())
		throw "Iterator is closed";
	size_t pos = fp->tellg();
	fp->seekp(pos + LINE_SIZE, fp->beg);
	FileSystem::write_line(fp, new_fd->GetName(), strlen(new_fd->GetName()));
	FileSystem::write_line(fp, new_fd->GetType(), strlen(new_fd->GetType()));
	FileSystem::write_line(fp, new_fd->GetSize());
	FileSystem::write_line(fp, new_fd->GetFormatDate(), 11);
	FileSystem::write_line(fp, new_fd->GetOffset());
	fp->seekp(pos, fp->beg);
}

FileDescriptor * FileIterator::GetFileDescriptor(){
	if (!fp->is_open())
		throw "Iterator is closed";
	return current;
}

void FileIterator::read_fd(){
	if (!fp->is_open())
		throw "Iterator is closed";
	current = new FileDescriptor();
	size_t pos = fp->tellg();
	fp->seekg(pos + LINE_SIZE, fp->beg);
	current->SetName(FileSystem::read_line(fp));
	current->SetType(FileSystem::read_line(fp));
	current->SetSize(atol(FileSystem::read_line(fp)));
	current->SetFormatDate(FileSystem::read_line(fp));
	current->SetOffset(atol(FileSystem::read_line(fp)));
	next = atoi(FileSystem::read_line(fp));
	fp->seekg(pos, fp->beg);
}

void FileIterator::set_next(int index){
	if (!fp->is_open())
		throw "Iterator is closed";
	next = index;

	size_t pos = fp->tellg();
	fp->seekp(pos + (LINES_PER_FD - 2)*LINE_SIZE, fp->beg);
	FileSystem::write_line(fp, next);
	fp->seekp(pos, fp->beg);
}