#include "virtualMachine.h"
#include "lexicalAnalyzer.h"
#include "simpleVitualMachine.h"
#include "simpleVitualMachine.h"

void testSimpleVM(string fileName)
{
	simpleVitualMachine simpleVM(fileName);
	simpleVM.run();
}
void testLexAnalyzer(string fileName)
{
	lexicalAnalyzer lex(fileName);
	while (1)
	{
		lexicalAnalyzer::Token t = lex.next();
		if (t.term == lexicalAnalyzer::_OVER)
		{
			break;
		}
		t.disp();
	}

}



int main()
{
	testLexAnalyzer("../test/test.c");
	testSimpleVM("../test/testVirtualMachine.s");
	return 0;
}
