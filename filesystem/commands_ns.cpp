#include "commands_ns.h"

void commands_ns::stub(FileSystem *fs, int argc, char *argv[], std::ostream& out){
	out << "Name of tom: " << fs->GetTomName() << std::endl;
	out << "Owner's name: " << fs->GetOwner() << std::endl;
	out << "Max size: " << fs->GetMaxSize() << std::endl;
	out << "File's count: " << fs->GetFilesCount() << std::endl;
	out << "Arguments of this command: ";
	for (int i = 0; i < argc; ++i){
		out << argv[i];
		if (i != argc - 1)
			out << ", ";
	}
	out << std::endl;
	out << "File names: " << std::endl;

	FileIterator *iter = fs->GetIterator();
	FileDescriptor* fd;
	while (iter->HasNext()){
		iter->Next();
		fd = iter->GetFileDescriptor();
		out << fd->GetName() << std::endl;
		delete fd;
	}
}

void commands_ns::Exit(FileSystem *fs, int argc, char *argv[], std::ostream& out){
	exit(0);
}