#include <iostream>
#include <vector>

class InputOutput
{

public:
	InputOutput(std::istream& in, std::ostream& out);
	~InputOutput();
	std::istream& GetInput();
	std::ostream& GetOutput();
	std::vector<char*>* ReadLine(char* separators); //Читает строку, и разбивает её на подстроки(по указанным разделителям)
	void InputOutput::WriteLine(char *line); //Пишет строку
	void InputOutput::Parse(char* args, char* separators, std::vector<char*>* argv); //Разбивает строку на подстроки(по указанным разделителям)
																					 //и кладет подстроки в вектор

private:
	std::istream& in;
	std::ostream& out;

};