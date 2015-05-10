#include "FileIterator.h"
#include "FileDescriptor.h"
#include "FileSystem.h"
void List(FileSystem *fs, int argc, char *argv[], ostream& out){
//будет примерно так
  FileIterator* fi=fs->GetIterator();
  for(int i=0;i<fi->GetFilesCount();i++){
    FileDescriptor* fd=fi->GetFileDescriptor();
    out<<fd->GetName();
    fi->Next();
  }

}
