//��� ����� ��� ������ ���������

#define LINE_SIZE 100	//������ ������������� ������ � �������� �����(fs.txt)

#define FILE_NAME "fs.txt"	//�������� ����� �� ���������

#define MAX_FILES_COUNT 31	//������������ ����������� ������ � �������

#define LINES_PER_FD 8 // ����������� ����� ��� �������� ������ ��������� ����������� � �����(fs.txt)

#define META_END 6 * LINE_SIZE // ������ ������� �������� ������������ � �����(fs.txt) 

#define START_OF_FILE_SPACE MAX_FILES_COUNT*LINES_PER_FD*LINE_SIZE+META_END //������ ������� ������ ������

#define R_FREE	"001000" //������ � ��������� �������

#define R_CONST	"002000" //������ � ���������� �����

#define R_TEMP	"000400" //������ � ��������� �����
