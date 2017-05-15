/*
* @Author: Leif
* @Email: leifzhu@foxmail.com
* @Date:   2017-05-14 21:00:02
* @Last Modified by:   Leif
* @Last Modified time: 2017-05-14 21:01:52
*/
#include "lexicalAnalyzer.h"
#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	if(argc == 1)
	{
		string filename;
		cout<<"please input file name (path should be included):"<<endl;
		cin>>filename;
		lexicalAnalyzer lexer(filename);
		lexer.analyze();
	}
	else if(argc == 2)
	{
		lexicalAnalyzer lexer((string(argv[1])));
		lexer.analyze();
	}
	else
	{
		cout<<"usage: ./lexer [filename]"<<endl;
	}
	return 0;
}
