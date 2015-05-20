#define _CRT_SECURE_NO_WARNINGS
#include "commands_ns.h"
#include "const.h"
#include <list>

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
	if (argc!=0){
		out<<"Неправильное количество параметров"<<std::endl;
		return;
	}
	exit(0);
}
/*Модуль вывода оглавления в порядке, в котором файлы представлены в системе 
Автор: Павлов Николай
write in Microsoft  Visual Studio 2010*/
void commands_ns::List(FileSystem *fs, int argc, char *argv[], std::ostream& out){
  if(argc!=0) {
  	out<<"Неправильное количество параметров"<<std::endl;
	return;
  }
  FileIterator* fi=fs->GetIterator();
  if(fs->GetFilesCount()!=0){
	  out<<"Обнаруженные файлы:"<<std::endl;
	  while(fi->HasNext()){
		 fi->Next();
		 FileDescriptor* fd=fi->GetFileDescriptor();
		 out<<'\t';
		 out<<fd->GetName()<<"."<<fd->GetType()<<std::endl;
	 }
  }
  else 
	  out<<"Файлов в системе не обнаружено."<< std::endl;

}
/*Модуль вывода оглавления в алфавитном порядке
Автор: Павлов Николай
write in Microsoft  Visual Studio 2010*/
void commands_ns::Lista(FileSystem *fs, int argc, char *argv[], std::ostream& out){
  if(argc!=0) {
  	out<<"Неправильное количество параметров"<<std::endl;
	return;
  }
   FileIterator* fi=fs->GetIterator();
   int N=fs->GetFilesCount();
   char** spisok=new char*[N];
   if(N!=0){
	   out<<"Обнаруженные файлы:"<<std::endl;
	   int j=0;
       while(fi->HasNext()){
		   fi->Next();
		   FileDescriptor* fd=fi->GetFileDescriptor();
		   spisok[j]=strcat(fd->GetName(),".");
		   spisok[j]=strcat(spisok[j],fd->GetType());
	       j++;
       }

//алгоритм сортировки
       char temp[40]; 
       for (int i=1;i<N;i++){
		   for (int j=0;j<N-1;j++){
			   if (strcmpi(spisok[i],spisok[j])<0){
				   strcpy(temp,spisok[i]);
				   strcpy(spisok[i],spisok[j]);
				   strcpy(spisok[j],temp);
               }
           }
       }
//вывод построенного списка имен
	   for(int i=0;i<N;i++){
		   out<<'\t';
	       out<<spisok[i]<< std::endl;
       }
  }
  else 
	  out<<"Файлов в системе не обнаружено."<< std::endl;
}

void commands_ns::DiskInfo(FileSystem *fs, int argc, char *argv[], std::ostream& out){	
	// будем считать, что размеры файлов даны в kb
	size_t totalUsedSpace = 0;
	size_t fileSystemSize = fs->GetMaxSize();
	FileIterator* fileIterator = fs->GetIterator();
	
	while(fileIterator->HasNext()){
		fileIterator->Next();
		FileDescriptor* fileDescriptor = fileIterator->GetFileDescriptor();
		totalUsedSpace += fileDescriptor->GetSize();
	}

	size_t freeDiskSpace = fileSystemSize - totalUsedSpace;
	double ratio = (double)freeDiskSpace/fileSystemSize; 
	
	/*
	out << "Сведения о диске:" << std::endl;
	out << "Наименование: " << fs->GetTomName() << std::endl;
	out << "Размер свободного места: " << freeDiskSpace << " кб (" << ratio*100 << "%)" << std::endl;
	out << "Максимальный объем: " << fileSystemSize << " кб" << std::endl;
	*/

	out << "Disk info:" << std::endl;
	out << "Name: " << fs->GetTomName() << std::endl;
	out << "Free disk space: " << freeDiskSpace << " kb (" << ratio*100 << "%)" << std::endl;
	out << "Max size: " << fileSystemSize << " kb" << std::endl;
}

typedef struct tagMemList
{
	FileDescriptor *FDescrPtr;
	size_t offset;
	size_t sz;
	unsigned int index;
} MemList;
void commands_ns::Cmprs(FileSystem *fs, int argc, char *argv[], std::ostream& out)
{
	/*/тестовая последовательность блоков==============
	FileDescriptor fd;
	fd.SetName("fd_name");
	fd.SetType("fd_type");
	MemList ml;
	std::list < MemList> List;
	std::list<MemList>::iterator iter;
	size_t prevOffst = 0;
	size_t prevSz = 0;
	FileDescriptor *prevFD = &fd;
	srand(10);
	for (int i = 0; i < 15; i++)
	{
		int r = (prevFD)?(rand() % 2):1;
		ml.FDescrPtr = (r == 1) ? &fd : NULL;
		prevFD = ml.FDescrPtr;
		ml.offset = prevOffst + prevSz;
		prevSz = 1 + (rand() % 25);
		ml.sz = prevSz;
		prevOffst = ml.offset;
		List.push_back(ml);
	}
	//============================================*/
	if (fs->GetFilesCount() <= 0) { out << "no files" << std::endl; return; }//проверяем наличие файлов
	//формируем список блоков из дескрипторов по возрастанию смещения======
	std::list < MemList> List;
	std::list<MemList>::iterator iter;
	FileIterator *fi = fs->GetIterator();
	FileDescriptor *fd;
	MemList ml;
	unsigned int indx = 0, max_indx;
	do
	{
		fi->Next();
		fd = fi->GetFileDescriptor();
		ml.FDescrPtr = fd;
		ml.offset = fd->GetOffset();
		ml.sz = fd->GetSize();
		ml.index = indx;
		if (List.empty())
		{
			List.push_back(ml);
		}
		else
		{
			for (iter = List.begin(); iter != List.end(); iter++)
			{
				if (ml.offset < (*iter).offset)
				{
					List.insert(iter, ml);
					break;
				}
			}
			if (iter == List.end())
			{
				List.push_back(ml);
			}
		}
		++indx;
	} while (fi->HasNext());
	max_indx = indx - 1;
	//вставляем пустой блок в начало, если нужно
	size_t FD_END = MAX_FILES_COUNT*LINES_PER_FD*LINE_SIZE + META_END;
	if ((*(List.begin())).offset > FD_END)
	{
		ml.offset = FD_END;
		ml.sz = (*(List.begin())).offset - FD_END;
		ml.FDescrPtr = NULL;
		List.insert(List.begin(), ml);
	}
	//вставляем пустые блоки в список
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if (std::next(iter) != List.end())
		{
			if ((*(std::next(iter))).offset != ((*iter).offset + (*iter).sz))
			{
				ml.offset = (*iter).offset + (*iter).sz;
				ml.sz = (*(std::next(iter))).offset - ml.offset;
				ml.FDescrPtr = NULL;
				List.insert(std::next(iter), ml);
				++iter;
			}
		}
	}
	//=========================================================*/
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		out << "offset: " << (*iter).offset << "; size: " << (*iter).sz << ((*iter).FDescrPtr ? "; file" : "; empty block");
		if ((*iter).FDescrPtr) { out << ", name: " << (*iter).FDescrPtr->GetName(); }
		out << std::endl;
	}
	out << "The whole data size before compressing: " << ( (*(std::prev(List.end()))).offset + (*(std::prev(List.end()))).sz - (*(List.begin())).offset) << std::endl;
	//сжатие===================================================
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if (!((*iter).FDescrPtr))//если текущий блок пустой
		{
			if (std::next(iter) != List.end())//если не последний
			{
				//ищем наибольший влезающий в текущий пустой блок с конца
				std::list<MemList>::iterator MaxAppr = List.end();
				for (std::list<MemList>::iterator insIter = std::prev(List.end()); insIter != iter; insIter--)
				{
					if ((*insIter).FDescrPtr)
					{
						if ( ((MaxAppr == List.end()) || (((*insIter).sz > (*MaxAppr).sz))) && ((*insIter).sz <= (*iter).sz) )
						{
							MaxAppr = insIter;
						}
					}
				}
				if (MaxAppr != List.end())//если нашёлся
				{
					MemList mlCopy = (*MaxAppr);//копируем
					//делаем перемещаемый блок пустым
					(*MaxAppr).FDescrPtr = NULL;
					if ( (std::next(MaxAppr) != List.end()) && ((*(std::next(MaxAppr))).FDescrPtr == NULL) )//если следующий пустой - объединяем
					{
						size_t rmv_size = (*(std::next(MaxAppr))).sz;
						List.erase(std::next(MaxAppr));
						(*MaxAppr).sz += rmv_size;
					}
					if ((*(std::prev(MaxAppr))).FDescrPtr == NULL)//аналогично с предыдущим
					{
						size_t rmv_size = (*MaxAppr).sz;
						std::list<MemList>::iterator li = std::prev(MaxAppr);
						(*li).sz += rmv_size;
						List.erase(MaxAppr);
					}
					//вставляем копию перед текущим пустым блоком
					mlCopy.offset = (*iter).offset;
					List.insert(iter, mlCopy);
					//корректируем смещение и размер пустого блока после вставки
					(*iter).sz -= mlCopy.sz;
					(*iter).offset += mlCopy.sz;
					//если перемещаемый блок полностью заполнил пустой блок - удаляем текущий пустой блок
					if ((*iter).sz == 0)
					{
						std::list<MemList>::iterator li = std::prev(iter);
						List.erase(iter);
						iter = li;//ставим указатель на предыдущий
					}
					else//если пустое место осталось
					{
						if ( (std::next(iter) != List.end()) && ((*(std::next(iter))).FDescrPtr == NULL) )//если следущий пуст - объединяем
						{
							size_t rmv_size = (*(std::next(iter))).sz;
							List.erase(std::next(iter));
							(*iter).sz += rmv_size;
						}
						--iter;//ставим указатель на предыдущий (он существует, так как только что вставили)
					}
				}
				else//если не нашёлся блок для вставки
				{
					//удаляем текущий пустой блок
					size_t rmv_size = (*iter).sz;
					std::list<MemList>::iterator insIter = List.erase(iter);
					//уменьшаем смещение всех последующих до пустого блока
					for (; ((*insIter).FDescrPtr && (insIter != List.end())); insIter++)
					{
						(*insIter).offset -= rmv_size;
					}
					//если встретился пустой блок
					if (insIter != List.end())
					{
						(*insIter).offset -= rmv_size;//уменьшаем смещение
						(*insIter).sz += rmv_size;//увеличиваем размер
						iter = insIter;//ставим указатель на предыдущий блок
						--iter;
					}
					else//если нет больше пустых блоков - завершаем цикл
					{
						break;
					}
				}
			}
			else//если последний
			{
				List.erase(iter);//удаляем
				break;//выходим из цикла
			}
		}
	}
	//выводим список и записываем изменения в файл================
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		out << "offset: " << (*iter).offset << "; size: " << (*iter).sz << ((*iter).FDescrPtr ? "; file" : "; empty block");
		if ((*iter).FDescrPtr) { out << ", name: " << (*iter).FDescrPtr->GetName(); }
		out << std::endl;
		(*iter).FDescrPtr->SetOffset((*iter).offset);//меняем смещение в дескрипторе
	}
	fi = fs->GetIterator();
	for (indx = 0; indx <= max_indx; indx++)
	{
		for (iter = List.begin(); ( (iter != List.end()) && ((*iter).index != indx) ); iter++) {}
		fi->Next();
		fi->SetFileDescriptor((*iter).FDescrPtr);
	}
	fi->Close();
	//выводим результат
	out << "The whole data size after compressing: " << ( (*(std::prev(List.end()))).offset + (*(std::prev(List.end()))).sz - (*(List.begin())).offset) << std::endl;
}
