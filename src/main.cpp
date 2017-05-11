#include <lexicalAnalyzer.h>

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



int main()
{
	testLexAnalyzer("../test/test.c");
	// testSimpleVM("../test/testVirtualMachine.s");
	return 0;
}
