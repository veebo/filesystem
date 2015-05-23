#define _CRT_SECURE_NO_WARNINGS
#include "commands_ns.h"
#include "const.h"
#include <list>
#include <algorithm>


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
			   if (_strcmpi(spisok[i],spisok[j])<0){
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
	if (argc != 0) {
		out << "Неправильное количество параметров" << std::endl;
		return;
	}
	//размеры файлов даны в байтах
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
	
	out << "Сведения о диске:" << std::endl;
	out << "Наименование: " << fs->GetTomName() << std::endl;
	out << "Размер свободного места: " << freeDiskSpace << " б (" << ratio*100 << "%)" << std::endl;
	out << "Максимальный объем: " << fileSystemSize << " б" << std::endl;

}

void commands_ns::ChFsInfo(FileSystem *fs, int argc, char *argv[], std::ostream& out)
{
	if (argc != 2)
	{
		out << "Неверное количество параметров"<< std::endl;
		return;
	}
	else
	{
		if (fs->names_types(argv[0]) || fs->names_types(argv[1]))
		{
			out << "Некорректные данные" << std::endl;
			return;
		}
		else
		{
			fs->SetTomName(argv[0]);
			fs->SetOwner(argv[1]);
			out << "Текущая метка тома:    " << fs->GetTomName() <<std::endl 
				<< "Текущее имя владельца: " << fs->GetOwner() << std::endl
				<< "Изменение информации о ФС успешно" << std::endl;
		}
	}
}

void  commands_ns::Format(FileSystem *fs, int argc, char *argv[], std::ostream& out) // модуль не доработан жду модуля Delete()
{
	if (argc != 3)
	{
		out << "Неверное количество параметров"<< std::endl;
		return;
	}
	else
	{
		//size_t szt = atoi(argv[2]);
		
		if (fs->names_types(argv[0]) || fs->names_types(argv[1]) || fs->fssize(argv[2]))
		{
			out << "Некорректные данные" << std::endl;
			return;
		}
		else
		{
			FileIterator *fIter = fs->GetIterator();
			fs->SetTomName(argv[0]);
			fs->SetOwner(argv[1]);
			fs->SetMaxSize(atoi(argv[2]));
			while (fIter->HasNext())
			{
				fIter->Next();
				//fIter->Delete();
			}
			fIter->Close();
			out << "Форматирование ФС успешно" << std::endl;
		}
	}
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
	if (argc > 0) { out << "This command must be used without argumnents" << std::endl; return; }
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
	for (iter = List.begin(); iter != List.end(); iter++) { (*iter).FDescrPtr->SetOffset((*iter).offset); }//меняем смещение в дескрипторах
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

void commands_ns::MkFile(FileSystem *fs, int argc, char *argv[], std::ostream& out)
{
	if (argc != 1)
	{
		out << "Неверное количество параметров."<< std::endl;
		return;
	}
	if (strlen(argv[0]) > 10)
	{
		out << "Некорректные данные." << std::endl;
		return;
	};
	char *nt[2];
	nt[0] = strtok(argv[0], ".");
	nt[1] = strtok(NULL, " ,.-");
	if (fs->names_types(nt[0]) == 1)
	{
		out << "Некорректные данные."<< std::endl;
		return;
	}
	FileDescriptor* fd = new FileDescriptor();
	fd->SetName(nt[0]);
	fd->SetType(nt[1]);
	fd->SetSize(0);
	fs->CreateFile(fd);
}

void commands_ns::DelFile(FileSystem *fs, int argc, char *argv[], std::ostream& out)

{
	if (argc != 1)
	{
		out << "Неверное количество параметров." << std::endl;
		return;
	}
	if (strlen(argv[0]) > 10)
	{
		out << "Некорректные данные." << std::endl;
		return;
	};
	char *nt[2];
	nt[0] = strtok(argv[0], ".");
	nt[1] = strtok(NULL, " ,.-");
	if (fs->names_types(nt[0]) == 1)
	{
		out << "Некорректные данные." << std::endl;
		return;
	}
	FileIterator* fi = fs->GetIterator();
	FileIterator* prevfi;
	int j = 0;
	if (fs->GetFilesCount() != 0)
	{
		while (fi->HasNext())
		{
			
			FileDescriptor* fd = fi->GetFileDescriptor();
			if (strcmp(fd->GetName(), nt[0]) == 0
				&& strcmp(fd->GetType(), nt[1]) == 0)
			{
				if (prevfi == NULL)
				{
					fs->set_first_file(1);
					fi->Delete();
					return;
				}
				prevfi->set_next(j+1);

			}
			prevfi = fi;
			fi->Next();	
			j++;
		}
	}
	else
		out << "Файлов в системе не обнаружено." << std::endl;

}

void commands_ns::RenFile(FileSystem *fs, int argc, char *argv[], std::ostream& out)
{
	FileIterator* fi = fs->GetIterator();
	int i = 0, count = strlen(argv[0]), flag = 1;
	char *arg_name = new char[count];
	if (argc != 2)
	{
		out << "Неправильное количество параметров" << std::endl;
		delete[] arg_name;
		return;
	}

	while (argv[0][i] != '.')
	{
		arg_name[i] = argv[0][i];
		++i;
		if (i > count - 2)
		{
			out << "Введены некорректные данные" << std::endl;
			delete[] arg_name;
			return;
		}
	}

	if (i == 0)
	{
		out << "Введены некорректные данные" << std::endl;
		delete[] arg_name;
		return;
	}

	arg_name[i] = '\0'; count -= i; ++i;
	char *arg_type = new char[count];
	count = 0;
	while (argv[0][i])
	{
		arg_type[count] = argv[0][i];
		++i;
		++count;
	}

	arg_type[count] = '\0';
	count = 0;

	if (fs->names_types(arg_name) || fs->names_types(arg_type)||fs->names_types(argv[1]))
	{
		out << "Введены некорректные данные" << std::endl;
		delete[] arg_name;
		delete[] arg_type;
		return;
	}

	if (fs->GetFilesCount() != 0)
	{
		while (fi->HasNext())
		{
			fi->Next();
			FileDescriptor* fd = fi->GetFileDescriptor();
			if (strcmp(fd->GetName(), arg_name) == 0 && strcmp(fd->GetType(), arg_type) == 0)
			{
				if (strcmp(arg_name,argv[1])==0){
					out << "Изменение имени прошло успешно" << std::endl;
					delete[] arg_name;
 					delete[] arg_type;
					return;
				}
				FileIterator* fi2 = fs->GetIterator();
				while (fi2->HasNext())
				{
					fi2->Next();
					FileDescriptor* fd2 = fi2->GetFileDescriptor();
					if (strcmp(fd2->GetName(), argv[1]) == 0 && strcmp(fd2->GetType(), arg_type) == 0)
						{
							out << "Файл с таким именем и типом уже существует" << std::endl;
							delete[] arg_name;
 							delete[] arg_type;
							return;
						}
				}
				flag = 0;
				fd->SetName(argv[1]);
				fi->SetFileDescriptor(fd);
				out << "Изменение имени прошло успешно" << std::endl;
				delete[] arg_name;
 				delete[] arg_type;
				return;
			}
			++count;
		}
	}
	if (flag == 1)
	{
		out << "Файла в системе не обнаружено" << std::endl;
	}

	delete[] arg_name;
 	delete[] arg_type;
}

void commands_ns::ChType(FileSystem *fs, int argc, char *argv[], std::ostream& out)
{
	FileIterator* fi = fs->GetIterator();
	int i = 0, count = strlen(argv[0]), flag = 1;
	char *arg_name = new char[count];
	if (argc != 2)
	{
		out << "Неправильное количество параметров" << std::endl;
		delete[] arg_name;
		return;
	}

	while (argv[0][i] != '.')
	{
		arg_name[i] = argv[0][i];
		++i;
		if (i > count - 2)
		{
			out << "Введены некорректные данные" << std::endl;
			delete[] arg_name;
			return;
		}
	}

	if (i == 0)
	{
		out << "Введены некорректные данные" << std::endl;
		delete[] arg_name;
		return;
	}

	arg_name[i] = '\0'; count -= i; ++i;
	char *arg_type = new char[count];
	count = 0;
	while (argv[0][i])
	{
		arg_type[count] = argv[0][i];
		++i;
		++count;
	}

	arg_type[count] = '\0';
	count = 0;

	if (fs->names_types(arg_name) || fs->names_types(arg_type)||fs->names_types(argv[1]))
	{
		out << "Введены некорректные данные" << std::endl;
		delete[] arg_name;
		delete[] arg_type;
		return;
	}

	if (fs->GetFilesCount() != 0)
	{
		while (fi->HasNext())
		{
			fi->Next();
			FileDescriptor* fd = fi->GetFileDescriptor();
			if (strcmp(fd->GetName(), arg_name) == 0 && strcmp(fd->GetType(), arg_type) == 0)
			{
				if (strcmp(arg_type,argv[1])==0){
					out << "Изменение имени прошло успешно" << std::endl;
					delete[] arg_name;
 					delete[] arg_type;
					return;
				}
				FileIterator* fi2 = fs->GetIterator();
				while (fi2->HasNext())
				{
					fi2->Next();
					FileDescriptor* fd2 = fi2->GetFileDescriptor();
					if (strcmp(fd2->GetName(), arg_name) == 0 && strcmp(fd2->GetType(), argv[1]) == 0)
						{
							out << "Файл с таким именем и типом уже существует" << std::endl;
							delete[] arg_name;
							delete[] arg_type;
							return;
						}
				}
				flag = 0;
				fd->SetType(argv[1]);
				fi->SetFileDescriptor(fd);
				out << "Изменение имени прошло успешно" << std::endl;
				delete[] arg_name;
 				delete[] arg_type;
				return;
			}
			++count;
		}
	}
	if (flag == 1)
	{
		out << "Файла в системе не обнаружено" << std::endl;
	}

	delete[] arg_name;
 	delete[] arg_type;
}



void commands_ns::AddToFile(FileSystem *fs, int argc, char *argv[], std::ostream& out)
{
	size_t size = 0;
	if (argc != 2)
	{
		out << "Неверное количество параметров." << std::endl;
		return;
	}

	char *nt[2];
	nt[0] = strtok(argv[0], ".");
	nt[1] = strtok(NULL, " ,.-");
	if (fs->names_types(nt[0]) == 1)
	{
		out << "Некорректные данные." << std::endl;
		return;
	};


	FileIterator* fi = fs->GetIterator();
	int N = fs->GetFilesCount();

	if (N != 0)
	{
		while (fi->HasNext())
		{
			fi->Next();
			FileDescriptor* fd = fi->GetFileDescriptor();

			size = (size + fd->GetSize());

		};


	};

	size_t e = fs->GetMaxSize();
	if ((e - size)  <= 0)
	{
		out << "Превышен максимальный объем." << std::endl;
	};

	if (N != 0)
	{
		while (fi->HasNext())
		{
			fi->Next();
			FileDescriptor* fd = fi->GetFileDescriptor();
			if (strcmp(fd->GetName(), nt[0]) != 0
				&& strcmp(fd->GetType(), nt[1]) != 0)
			{
				out << "Файл не найден." << std::endl;
				return;
			}
			else
			{
				FileIterator* cur = fi;

				if (fd->GetSize() != 0)
				{
					int j = 0;
					int jmin = 0;
					size_t min;

					std::vector<int> space;

					space.push_back(START_OF_FILE_SPACE);
					space.push_back(fs->GetMaxSize());

					FileIterator* iter = fs->GetIterator();
					FileDescriptor* fd;

					int i = 1;

					int destoffset;
					while (iter->HasNext()){
						iter->Next();
						fd = iter->GetFileDescriptor();

						space.push_back(fd->GetOffset());

						space.push_back(fd->GetOffset() + fd->GetSize());
						delete fd;
						++i;
					}

					//сортируем вектор
					std::sort(space.begin(), space.end());

					//итератор по вектору
					std::vector<int>::iterator it = space.begin();

					//Тут будем хранить предыдущий оффсет
					int prevOffset = *it;

					++it;

					i = 0;
					for (; it != space.end(); ++it){

						//Проверяем только четные значения i, т.к. там будут свободные места
						if (i % 2 == 0){
							if (*it - prevOffset >= size){

								destoffset = prevOffset;
								fd = cur->GetFileDescriptor();
								fd->SetSize(strlen(argv[1]));
								fd->SetOffset(destoffset);
								cur->SetFileDescriptor(fd);

							}
						}

						prevOffset = *it;
						++i;
					}
					return;

				}
				else
				{
					fd->SetSize(strlen(argv[1]));
				}
			};
		}
	}
}



