#include <virtualMachine.h>
#include <lexicalAnalyzer.h>

// int main(int argc, char **argv)
// {
// 	list<int> inst;
// 	virtualMachine VM;
// 	inst = VM.readInstrutionsFromFile("tt.txt");
// 	VM.run(inst);
// 	return 0;
// }


int main()
{
	cout << "test" << endl;
	lexicalAnalyzer lex("../test/test.c");
	while (1)
	{
		lexicalAnalyzer::Token t = lex.next();
		if(t.term == lexicalAnalyzer::_OVER)
		{
			break;
		}
		t.disp();
	}
	return 0;
}
