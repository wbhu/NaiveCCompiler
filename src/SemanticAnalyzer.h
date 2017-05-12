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
 	void analyze();
 	void program();
 	void compound_stat();
 	void statement();
 	void expression_stat();
 	void expression();
 	void bool_expr();
 	void additive_expr();
 	void term();
 	void factor();
 	void if_stat();
 	void while_stat();
 	void for_stat();
 	void print_stat();
 	void read_stat();
 	void declaration_stat();
 	void declaration_list();
 	void statement_list();
 	void name_def(string &name);
 	void lookup(string &name,int &address);
 	void print_vartable();
 	void get(Token &tk);
 	inline bool match(Term terminal);
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
