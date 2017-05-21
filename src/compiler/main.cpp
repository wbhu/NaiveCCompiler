/*
* @Author: Leif
* @Email: leifzhu@foxmail.com
* @Date:   2017-05-11 19:56:08
* @Last Modified by:   Leif
* @Last Modified time: 2017-05-21 14:57:59
*/
#include "lexicalAnalyzer.h"
#include "SemanticAnalyzer.h"
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	if (argc == 3 && strcmp(argv[2], "-l") == 0) //ncc file -l
	{
		lexicalAnalyzer lexer((string(argv[1])));
		Token temp = lexer.next();
		while (temp.term != _OVER)
		{
			temp.disp();
			temp = lexer.next();
		}
	}
	else if (argc == 2) // ncc file
	{
		SemanticAnalyzer analyzer((string(argv[1])));
		analyzer.analyze();
	}
	else if (argc == 4 && strcmp(argv[2], "-o") == 0) //ncc inputfile -o outputfile
	{
		SemanticAnalyzer analyzer((string(argv[1])), (string(argv[3])));
		analyzer.analyze();
	}
	else
	{
		cout << "usage:ncc inputfile [-l | -o outputfile]" << endl;
	}
	return 0;
}
