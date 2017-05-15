/*
* @Author: Leif
* @Email: leifzhu@foxmail.com
* @Date:   2017-05-11 14:48:58
* @Last Modified by:   Leif
* @Last Modified time: 2017-05-15 15:54:19
*/
#ifndef _SEMATICANALYZER_H_
#define _SEMATICANALYZER_H_

#include <string>
#include <fstream>
#include <vector>
#include "lexicalAnalyzer.h"
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;


class SemanticAnalyzer
{
	struct Record
	{
		string name;
		int address;
		Record(string &name, int address)
		{
			this->name = name;
			this->address = address;
		}
	};
	Token token;
	lexicalAnalyzer lexer;
	string codeOut;
	ofstream fout;
	ifstream fin;
	vector<Record> varTable;
	int labelTag;
	int	dataAddress;

public:
	SemanticAnalyzer(string input);
	SemanticAnalyzer(string input, string output);
	~SemanticAnalyzer();
	inline bool match(Term terminal);
	inline void stop();
	inline void pop();
	inline void jmp(int label);
	inline void jz(int label);
	inline void operate(Term op);
	inline void store(int address);
	inline void setLabel(int label);
	inline void load(int address);
	inline void imm(string constnum);
	inline void in();
	inline void out();
	void analyze();
	void program();
	void compoundStat();
	void statement();
	void expressionStat();
	void expression();
	void boolExpr();
	void additiveExpr();
	void term();
	void factor();
	void ifStat();
	void whileStat();
	void forStat();
	void printStat();
	void readStat();
	void declarationStat();
	void declarationList();
	void statementList();
	void nameDef(string &name);
	void lookup(string &name,int &address);
	void printVartable();
	void get(Token &tk);
};

#endif
