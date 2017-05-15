/**
 *
 * @author:             胡文博
 * @email:              huwenbo@mail.dlut.edu.cn
 * @dateTime:           2017-05-08 21:28:48
 * @description:
 */
#ifndef _LEXICALANALYZER_
#define _LEXICALANALYZER_

#include <string>
#include <fstream>
#include <map>
#include <vector>
using namespace std;


enum Term
{
	_MAIN,
	_INT,
	_WHILE,
	_IF,
	_ELSE,
	_RETURN,
	_VOID,

	_PRINT,
	_READ,
	_FOR,
	
	_NOT,			// !
	_ADD,           		// +
	_SUB,            		// -
	_MUL,           		// *
	_DIV,           		// /
	_MOD,         		 // %
	_BIGGER,        		// >
	_SMALLLER,      		// <
	_COMMA,     		// ,
	_SEMICOLON, 		// ;
	_BRACE_L,       		// {
	_BRACE_R,       		// }
	_PARENTHESE_L,   	// (
	_PARENTHESE_R,       	// )
	_BRACKET_L,     		// [
	_BRACKET_R,     	// ]
	_ASSIGN,        		// =
	_EQUAL,		// ==
	_BIGGEROREQUAL,	//>=
	_SIMMALLEROREQUAL, //   <=
	_NOTEQUAL,		// !=
	_ID,
	_NUM,
	_OVER				//the flag of source over
};


class Token
{
public:
	Token()
	{
		term = _OVER;
		value = "";
	}
	Term term;
	string value;
	void disp();
};




class lexicalAnalyzer
{
	//存放处理后的字符串
	char tempstr[255] = {};
	//空格标志
	bool temp = false;
	//临时数组
	char word[255] = {};
	ifstream file;
	char *str = NULL;     //存放每行的字符串
	char *delims = (char*)" ";  //分割标志
	string source;
	bool minusOrNegtiveFlag = false;
	int backStepNum = 0;
	char symbol[17] = { '!', '+', '-', '*', '/', '%', '>',  '<', ',' , ';', '{' , '}',  '(', ')', '[', ']', '='};
	static const int keywordNum = 10;
	string keyword[keywordNum] =
	{
		"main",   "int", "while", "if",  "else", "return", "void", "print", "read", "for"
	};
	
	bool IsLetter(char ch);
	bool IsDigit(char ch);
	// int IsSymbol(char ch);
	int IsKeyword(string str);
	void HandleSpace(const char a[]);
	void prePro();


public:
	// lexicalAnalyzer();
	lexicalAnalyzer(string fileName);
	~lexicalAnalyzer();
	Token next();
	Token last();
private:
	vector<Token> tokenStream;

};

#endif
