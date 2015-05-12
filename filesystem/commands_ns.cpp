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
void commands_ns::List(FileSystem *fs, int argc, char *argv[], std::ostream& out){
  FileIterator* fi=fs->GetIterator();
  if(fs->GetFilesCount()!=0){
	  out<<"Обнаруженные файлы:"<<std::endl;
  while(fi->HasNext()){
	fi->Next();
    FileDescriptor* fd=fi->GetFileDescriptor();
	out<<'\t';
    out<<fd->GetName()<< std::endl;
  }
  }
  else 
	  out<<"Файлов в системе не обнаружено."<< std::endl;

}
void commands_ns::Lista(FileSystem *fs, int argc, char *argv[], std::ostream& out){
   FileIterator* fi=fs->GetIterator();
  int N=fs->GetFilesCount();
  char** spisok=new char*[N];
  if(N!=0){
	  out<<"Обнаруженные файлы:"<<std::endl;
	int j=0;
 while(fi->HasNext()){
	fi->Next();
    FileDescriptor* fd=fi->GetFileDescriptor();
	spisok[j]=fd->GetName();
	j++;
  }
  char temp[20]; 
  for (int i=1;i<N;i++){
    for (int j=0;j<N-1;j++){
        if (strcmp(spisok[i],spisok[j])<0){
            strcpy(temp,spisok[i]);
            strcpy(spisok[i],spisok[j]);
            strcpy(spisok[j],temp);
        }
    }
  }
  for(int i=0;i<N;i++){
	out<<'\t';
	out<<spisok[i]<< std::endl;
  }
  }
  else 
	  out<<"Файлов в системе не обнаружено."<< std::endl;
}
