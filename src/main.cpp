#include <virtualMachine.h>

int main(int argc, char **argv)
{
	list<int> inst;
	virtualMachine VM;
	inst = VM.readInstrutionsFromFile("tt.txt");
	VM.run(inst);
	return 0;
}
