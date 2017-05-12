#include "lexicalAnalyzer.h"
#include "SemanticAnalyzer.h"
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;

// void testSimpleVM(string fileName)
// {
// 	simpleVitualMachine simpleVM(fileName);
// 	simpleVM.run();
// }
/*void testLexAnalyzer(string fileName)
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
*/
/*
 void testSemanticAnalyzer(string filename)
{
    SemanticAnalyzer sa(filename);
    sa.analyze();
}
 */



int main(int argc, char** argv)
{
    if(argc == 3 && strcmp(argv[2],"-l") == 0) //ncc file -l
    {
        lexicalAnalyzer lexer((string(argv[1])));
        Token temp = lexer.next();
        while(temp.term != _OVER)
        {
            temp.disp();
            temp = lexer.next();
        }
    }
    else if(argc == 2)// ncc file
    {
        SemanticAnalyzer analyzer((string(argv[1])));
        analyzer.analyze();
    }
    else if(argc == 4 && strcmp(argv[2],"-o") == 0)//ncc inputfile -o outputfile
    {
        SemanticAnalyzer analyzer((string(argv[1])),(string(argv[3])));
        analyzer.analyze();
    }
    else
    {
        cout<<"usage:ncc inputfile [-l | -o outputfile]"<<endl;
    }
	//testLexAnalyzer("../test/test.c");
    //testSemanticAnalyzer("../test/1.txt");
	return 0;
}
