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
 	string tokenStream;
 	string codeOut;
 	ofstream fout;
 	ifstream fin;
 	vector<Record> varTable;
 	int labelp;
	int	datap;

 public:
 	SemanticAnalyzer(string &input);
 	~SemanticAnalyzer();
 	int analyze();
 	int program();
 	int compound_stat();
 	int statement();
 	int expression_stat();
 	int expression();
 	int bool_expr();
 	int additive_expr();
 	int term();
 	int factor();
 	int if_stat();
 	int while_stat();
 	int for_stat();
 	int print_stat();
 	int read_stat();
 	int declaration_stat();
 	int declaration_list();
 	int statement_list();
 	int name_def(string &name);
 	int lookup(string &name,int &address);
 	void print_vartable();
 	void get(Token &tk);
 	inline void stop();
 	inline void pop();
 	inline void br(int label);
 	inline void brf(int label);
 	inline void operate(Term op);
 	inline void store(int address);
 	inline void set_label(int label);
 	inline void load(int address);
 	inline void loadi(string constnum);
 	inline void in();
 	inline void out();

 };

#endif
