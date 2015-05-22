#include <iostream>
#include <vector>

class InputOutput
{

public:
	InputOutput(std::istream& in, std::ostream& out);
	~InputOutput();
	std::istream& GetInput();
	std::ostream& GetOutput();
	std::vector<char*>* ReadLine(char* separators); //×èòàåò ñòðîêó, è ðàçáèâàåò å¸ íà ïîäñòðîêè(ïî óêàçàííûì ðàçäåëèòåëÿì)
	void InputOutput::WriteLine(char *line); //Ïèøåò ñòðîêó
	void InputOutput::Invite();
	void InputOutput::Parse(char* args, char* separators, std::vector<char*>* argv); //Ðàçáèâàåò ñòðîêó íà ïîäñòðîêè(ïî óêàçàííûì ðàçäåëèòåëÿì)
																					 //è êëàäåò ïîäñòðîêè â âåêòîð

private:
	std::istream& in;
	std::ostream& out;

};
