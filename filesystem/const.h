//Тут будут все важные константы

#define LINE_SIZE 100	//Размер фиксированной строки в текством файле(fs.txt)

#define FILE_NAME "fs.txt"	//Название файла по умолчанию

#define MAX_FILES_COUNT 31	//Максимальное колличество файлов в системе

#define LINES_PER_FD 8 // Колличество строк для описания одного файлового дескриптора в файле(fs.txt)

#define META_END 6 * LINE_SIZE // Начало области файловых дескрипторов в файле(fs.txt) 

#define START_OF_FILE_SPACE MAX_FILES_COUNT*LINES_PER_FD*LINE_SIZE+META_END //Начало области записи файлов

#define R_FREE	"001000" //Запись о свободной области

#define R_CONST	"002000" //Запись о постоянном файле

#define R_TEMP	"000400" //Запись о временном файле
