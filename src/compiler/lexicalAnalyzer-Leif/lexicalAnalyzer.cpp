/*
* @Author: Leif
* @Email: leifzhu@foxmail.com
* @Date:   2017-05-14 18:12:29
* @Last Modified by:   Leif
* @Last Modified time: 2017-05-22 01:27:18
*/
#include "lexicalAnalyzer.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
using std::ifstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

lexicalAnalyzer::lexicalAnalyzer(string inputFileName):
keyword{"if","else","for","while","int","read","print","main"},
delimiters("+-*(){};,:><=!"),
fin(inputFileName),
lineTag(1),
oldLineTag(1),
oldOffset(0),
ch('\0')
{}

lexicalAnalyzer::~lexicalAnalyzer(){}
Token lexicalAnalyzer::next()
{
	if(fin.eof()) throw 41; // unexpected end of file
	oldLineTag = lineTag;
	oldOffset = fin.tellg(); //record last line tag and offset of input file
	Token token;
	while(ch ==' '||ch == '\n'||ch == '\t') //filter space
	{
		if(ch == '\n') lineTag++;
		fin.get(ch);
		if(fin.eof()) return token;
	}

	if(isalpha(ch))
	{
		token.value.push_back(ch);
		fin.get(ch);
		if(!fin.eof())
		{
			while(isalnum(ch))
			{
				token.value.push_back(ch);
				fin.get(ch);
				if(fin.eof()) break;
			}
		}

		if(!fin.eof()&&find(delimiters.begin(),delimiters.end(),ch) ==  delimiters.end()
			&&ch != ' ' && ch != '\t' && ch != '\n') //123\n is OK but 123ab can't be accepted
		{
			throw 42; //illegal identifier
		}

		if(find(keyword.begin(),keyword.end(),token.value) == keyword.end())
		{
			token.type = "ID";
		}
		else
		{
			token.type = token.value;
		}
	}

	else if(isdigit(ch))
	{
		token.value.push_back(ch);
		fin.get(ch);
		if(!fin.eof())
		{
			while(isdigit(ch))
			{
				token.value.push_back(ch);
				fin.get(ch);
				if(fin.eof()) break;
			}
		}
		if(!fin.eof()&&find(delimiters.begin(),delimiters.end(),ch) ==  delimiters.end()
			&&ch != ' ' && ch != '\t' && ch != '\n')
		{
			throw 42; //illegal identifier
		}
		token.type = "NUM";
	}

	else 
	{
		string::iterator it = find(delimiters.begin(),delimiters.end(),ch);
		string::iterator itPartition = find(delimiters.begin(),delimiters.end(),'>');
		if(it < itPartition)
		{
			token.value.push_back(ch);
			token.type = token.value;
			fin.get(ch);
		}
		else if(it < delimiters.end())
		{
			token.value.push_back(ch);
			fin.get(ch);
			if(!fin.eof())
			{
				if(ch == '=')
				{
					token.value.push_back(ch);
					token.type = token.value;
					fin.get(ch);
				}
				else token.type = token.value;

			}
			else token.type = token.value;
		}
		else
		{
			throw 43; // unrecognizable token
		}
	}
	return token;
}

void lexicalAnalyzer::back()
{
	fin.seekg(oldOffset,std::ios::beg);
	lineTag = oldLineTag;
}

unsigned int lexicalAnalyzer::getLineTag()
{
	return lineTag;
}

void lexicalAnalyzer::analyze()
{
	try
	{
		if(!fin.is_open()) throw 40;//failed to open input file!
		fin.get(ch);
		while(!fin.eof())
		{
			Token tk = next();
			cout<<tk.type<<' '<<tk.value<<endl;
		}
		fin.close();
	}
	catch(int err)
	{
		cout<<"error: ";
		switch(err)
		{
			case 40:cout<<"failed to open input file!"<<endl;break;
			case 41:cout<<"line"<<lineTag<<": "<<"unexpected end of file!"<<endl;break;
			case 42:cout<<"line"<<lineTag<<": "<<"illegal identifier!"<<endl;break;
			case 43:cout<<"line"<<lineTag<<": "<<"unrecognizable token!"<<endl;break;
		}
	}
}
