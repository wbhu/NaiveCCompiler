/**
 * 
 * @author:		胡文博
 * @email:		huwenbo@mail.dlut.edu.cn
 * @dateTime:		2017-05-09 19:39:21
 * @description: 	
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include "lexicalAnalyzer.h"
using namespace std;
string TermNameSS[29] =
{
    "_MAIN",
    "_INT",
    "_WHILE",
    "_IF",
    "_ELSE",
    "_RETURN",
    "_VOID",
    "_ADD",
    "_SUB",
    "_MUL",
    "_MOD",
    "_REM",
    "_BIGGER",
    "_SMALLLER",
    "_COMMA" 		,
    "_SEMICOLON",
    "_BRACE_L",
    "_BRACE_R",
    "_PARENTHESE_L",
    "_PARENTHESE_R",
    "_BRACKET_L",
    "_BRACKET_R",
    "_ASSIGN",
    "_EQUAL"	,
    "_BIGGEROREQUAL",
    "_SIMMALLEROREQUAL",
    "_ID",
    "_NUM",
    "_OVER"
};

void lexicalAnalyzer::Token::disp()
{
    cout<<TermNameSS[term]<<"-----------------------------------"<<value<<endl;
}

// lexicalAnalyzer::lexicalAnalyzer()
// {
// 	cout << "please select a file to compile" << endl;
// }

lexicalAnalyzer::lexicalAnalyzer(string fileName)
{
	file.open(fileName);
	string s;
	while (getline(file, s))
	{
		source += s;
		source += ' ';
	}
	HandleSpace(source.c_str());
	prePro();
	str = strtok(tempstr, delims);
}

lexicalAnalyzer::~lexicalAnalyzer()
{
	file.close();
}

//判断是否为字母
bool lexicalAnalyzer:: IsLetter(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	return false;
}

//判断是否为数字,bug!!
bool lexicalAnalyzer:: IsDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return true;
	return false;
}

//判断是否为定界符等
int lexicalAnalyzer:: IsSymbol(char ch)
{
	for (int i = 0; i < 17; i++)
	{
		if (ch == symbol[i])
			return i;
	}
	return -1;
}

//判断是否为关键字
int lexicalAnalyzer:: IsKeyword(string str)
{
	for (int i = 0; i < 7; i++)
	{
		if (str == keyword[i])
		{
			return i;
		}
	}
	//不是关键字即为ID
	return -1;
}

//空格处理
void lexicalAnalyzer:: HandleSpace(const char a[])
{
	int j = 0;
	memset(word, 0, 255); //需要清空，不然可能残留上次的字符串
	temp = false;
	for (int i = 0; i < strlen(a); i++)
	{

		if (a[i] != ' ' && a[i] != '\t')
		{
			word[j++] = a[i];
			temp = false;
		}
		else
		{

			if (!temp && a[i] != '\t')
			{
				word[j++] = a[i];
				temp = true;
			}
		}
	}
}

//处理"//"注释//
void lexicalAnalyzer:: prePro()
{
	int j = 0;
	memset(tempstr, 0, 255);
	for (int i = 0; i < strlen(word); i++)
	{
		if (word[i] == '/' && word[i + 1] == '/')
		{
			while (i < strlen(word))
			{
				i++;
			}
		}

		else
		{
			tempstr[j++] = word[i];
		}
	}
}

lexicalAnalyzer::Token lexicalAnalyzer:: next()
{
	Token t;
	if (str == NULL)
	{
		cout << "The source code is over, don't call next() again!" << endl;

		return t;
	}
	string Tok;         //存放字符串

	bool flag = false;    //多行注释标志,false为未处于注释区域

	int i = 0;
	// if (*(str + i) == '/')
	// {
	// 	if (*(str + i + 1) == '*')
	// 	{
	// 		flag = true;
	// 		break;
	// 	}
	// }
	// //注释处理: */,注释区域结束
	// if (*(str + i) == '*' && flag)
	// {
	// 	if (*(str + i + 1) == '/')
	// 	{
	// 		flag = false;
	// 		i++;
	// 		break;
	// 	}
	// }
	//标识符，关键词
	if (IsLetter(*(str + i)) && (!flag))
	{
		while (IsLetter(*(str + i)) || IsDigit(*(str + i)) ||
		        *(str + i) == '_')
		{
			Tok += *(str + i);
			i++;
		}
		int keywordFlag = IsKeyword(Tok);
		if (keywordFlag != -1)
		{
			t.term = Term(keywordFlag);
			t.value = keyword[keywordFlag];

		}
		else
		{
			t.term = _ID;
			t.value = Tok;
		}
	}
	else if (IsDigit(*(str + i)) && (!flag))	//numbers
	{
		while (IsDigit(*(str + i)))
		{
			Tok += *(str + i);
			i++;
		}
		t.term = _NUM;
		t.value = Tok;
	}
	else
	{
		for (int index = 0; index < 16; index++)
		{
			if (*(str + i) == symbol[index] && (!flag))
			{
				if (*(str + i) == '=')
				{
					if (*(str + i + 1) == '=')
					{
						i += 2;
						t.term = _EQUAL;
						t.value = "==";
						break;
					}
					else
					{
						i++;
						t.term = _ASSIGN;
						t.value = "=";
						break;
					}
				}
				else if (*(str + i) == '>')
				{
					if (*(str + i + 1) == '=')
					{
						i += 2;
						t.term = _BIGGEROREQUAL;
						t.value = ">=";
						break;
					}
					else
					{
						i++;
						t.term = _BIGGER;
						t.value = ">";
						break;
					}
				}
				else if (*(str + i) == '<')
				{
					if (*(str + i + 1) == '=')
					{
						i += 2;
						t.term = _SIMMALLEROREQUAL;
						t.value = "<=";
						break;
					}
					else
					{
						i++;
						t.term = _SMALLLER;
						t.value = "<";
						break;
					}
				}

				else
				{
					i++;
					t.term = Term(index + 7);
					t.value = symbol[index];
					break;
				}
			}
		}
	}
	if (i < strlen(str))
	{
		str += i;
	}
	else
	{
		str = strtok(NULL, delims);
	}
	return t;
}

