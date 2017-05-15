/**
 *
 * @author:     胡文博
 * @email:      huwenbo@mail.dlut.edu.cn
 * @dateTime:       2017-05-10 16:33:00
 * @description:
 */
#include "simpleVitualMachine.h"
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <iostream>


// #define _DEBUG
using namespace std;

simpleVitualMachine::simpleVitualMachine()
{
	pc = 0;
	label.resize(100);
	data.resize(1024);
}

simpleVitualMachine::simpleVitualMachine(string fileName)
{
	pc = 0;
	label.resize(100);
	data.resize(1024);
	if ( readAsmFile(fileName))
	{
        cout << "No such file: " << fileName << endl;
		exit(-1);
	}
}

simpleVitualMachine::~simpleVitualMachine()
{

}
void simpleVitualMachine::dispCode()
{
	cout << "code is:" << endl;
	for (auto i = 0; i != code.size(); i++)
	{
		cout << i << ":" << code[i] << endl;
	}
}
void simpleVitualMachine::dispLabel()
{
	cout << "labels are:" << endl;
	for (auto i = 0; i < label.size(); i++)
	{

		cout << "label" << i << ":" << label[i] << endl;
	}
}

int simpleVitualMachine::readAsmFile(string fileName)
{
	fstream sourceFile;
	sourceFile.open(fileName);
	if(!sourceFile)
	{
		return 1;
	}
	string s;
	while (sourceFile >> s)
	{
#ifdef _DEBUG
		cout << "read file:" << s << endl;
#endif
		if (s.substr(0, 5) == "LABEL")
		{
			auto tmp = atoi(s.substr(5).data());
			label[tmp] = pc;
		}
		else
		{
			code.push_back(s);
			pc++;
		}
	}
#ifdef _DEBUG
	dispCode();
	dispLabel();
#endif
	return 0;
}

int simpleVitualMachine::run()
{
	pc = 0;
	for (; pc < code.size(); pc++)
	{
#ifdef _DEBUG
		cout << "code:" << code[pc] << endl;
#endif
		if (code[pc] == "LOAD")
		{
			calculateStack.push( data[ stoi(code[++pc]) ] );
		}
		else if (code[pc] == "IMM")
		{
			calculateStack.push(stoi(code[++pc]));
		}
		else if (code[pc] == "STO")
		{
			data[stoi(code[++pc])] = calculateStack.top();
		}
		else if (code[pc] == "POP")
		{
			calculateStack.pop();
		}
		else if (code[pc] == "ADD")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push(tmp1 + tmp2);
		}
		else if (code[pc] == "SUB")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push(tmp2 - tmp1);
		}
		else if (code[pc] == "MULT")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push(tmp1 * tmp2);
		}
		else if (code[pc] == "DIV")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 / tmp1);
		}
		else if (code[pc] == "JMP")
		{
			pc = label[ stoi(code[++pc]) ] - 1;
		}
		else if (code[pc] == "JZ")
		{
			int tmp = calculateStack.top();
			calculateStack.pop();
			pc++;
			if (tmp == 0)
				pc = label[ stoi(code[pc]) ]  - 1;
		}
		else if (code[pc] == "EQ")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 == tmp1);
		}
		else if (code[pc] == "NOTEQ")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 != tmp1);
		}
		else if (code[pc] == "GT")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 > tmp1);
		}
		else if (code[pc] == "LES")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 < tmp1);
		}
		else if (code[pc] == "GE")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 >= tmp1);
		}
		else if (code[pc] == "LE")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 <= tmp1);
		}
		else if (code[pc] == "AND")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 && tmp1);
		}
		else if (code[pc] == "OR")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			int tmp2 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( tmp2 || tmp1);
		}
		else if (code[pc] == "NOT")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			calculateStack.push( ! tmp1);
		}
		else if (code[pc] == "IN")
		{
			cout << "Please input an integer:" << endl;
			int tmp1 = 0;
			cin >> tmp1;
			calculateStack.push( tmp1);
		}
		else if (code[pc] == "OUT")
		{
			int tmp1 = calculateStack.top();
			calculateStack.pop();
			cout << "Program output: " << tmp1 << endl;
		}

	}
	return 0;
}
