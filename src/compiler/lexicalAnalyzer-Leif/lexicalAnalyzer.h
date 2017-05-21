/*
* @Author: Leif
* @Email: leifzhu@foxmail.com
* @Date:   2017-05-14 17:08:13
* @Last Modified by:   Leif
* @Last Modified time: 2017-05-21 15:01:26
*/
#ifndef _LEXICALANLYZER_H_
#define _LEXICALANLYZER_H_
#include <string>
#include <fstream>
#include <vector>
using std::ifstream;
using std::string;
using std::streamoff;
using std::vector;

typedef struct
{
	string type;
	string value;
}Token;

class lexicalAnalyzer
{
	vector<string> keyword; //keyword list
	string delimiters; //delimiters
	ifstream fin; // input file
	unsigned int lineTag;
	unsigned int oldLineTag; //for rollbacking one token
	streamoff oldOffset;
	char ch;
public:
	lexicalAnalyzer(string inputFileName);
	~lexicalAnalyzer();
	Token next(); //get next token
	void back(); // rollback one token
	unsigned int getLineTag(); // get current line tag
	void analyze(); //the entrance of lexical analysis
};
#endif
