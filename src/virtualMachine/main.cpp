#include <simpleVitualMachine.h>
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		cout<<"please add assembly file!"<<endl;
		return 0;
	}
	simpleVitualMachine VM(argv[1]);
	VM.run();
	return 0;
}
