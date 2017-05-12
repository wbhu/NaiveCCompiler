#include "lexicalAnalyzer.h"
#include "SemanticAnalyzer.h"

// void testSimpleVM(string fileName)
// {
// 	simpleVitualMachine simpleVM(fileName);
// 	simpleVM.run();
// }
void testLexAnalyzer(string fileName)
{
	lexicalAnalyzer lex(fileName);
	while (1)
	{
		Token t = lex.next();
		if (t.term == _OVER)
		{
			break;
		}
		t.disp();
		t= lex.next();
		if (t.term == _OVER)
		{
			break;
		}
		t.disp();
		t= lex.next();
		if (t.term ==  _OVER)
		{
			break;
		}
		t.disp();
		t = lex.last();
		if (t.term ==  _OVER)
		{
			break;
		}
		t.disp();
		t = lex.last();
		if (t.term ==  _OVER)
		{
			break;
		}
		t.disp();
	}

}

void testSemanticAnalyzer(string filename)
{
    SemanticAnalyzer sa(filename);
    sa.analyze();
}



int main()
{
	//testLexAnalyzer("../test/test.c");
	//testSimpleVM("../test/testVirtualMachine.s");
    testSemanticAnalyzer("../test/1.txt");
	return 0;
}
