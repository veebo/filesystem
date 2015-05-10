#include "FileIterator.h"
#include "FileDescriptor.h"
#include "FileSystem.h"
void List(FileSystem *fs, int argc, char *argv[], ostream& out){
//будет примерно так
  FileIterator* fi=fs->GetIterator();
  char fName[20];
  char** spisok=new fName[(fi->GetFilesCount())];
  for(int i=0;i<fi->GetFilesCount();i++){
    FileDescriptor* fd=fi->GetFileDescriptor();
    spisok[i]=fd->GetName();
    fi->Next();
  }
  TempfName=spisok[0];
  int k=-1;
  for(int i=0;i<fi->GetFilesCount();i++){
    if(TempfName>spisok[i]){
      TempfName=spisok[i];
      k=i;
    }
  }
  if(k>=0){
    spisok[k]='{';
    out<<TempfName;
  }
  
spisok=spisok+'\0';
out<<spisok;
}
