#include "FileIterator.h"
#include "FileDescriptor.h"
#include "FileSystem.h"
void List(FileSystem *fs, int argc, char *argv[], ostream& out){
//будет примерно так
  FileIterator* fi=fs->GetIterator();
  char* spisok=new char[15*(fi->GetFilesCount())];
  spisok='';
  for(int i=0;i<fi->GetFilesCount();i++){
    FileDescriptor* fd=fi->GetFileDescriptor();
    spisok=spisok+fd->GetName()+'\n';
    fi->Next();
  }
  //здесь алгоритм сортировки
  
spisok=spisok+'\0';
out<<spisok;
}
