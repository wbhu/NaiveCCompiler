#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "SemanticAnalyzer.h"
using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

inline bool SemanticAnalyzer::match(Term terminal)
{
	if(token.term == terminal) return true;
	else return false;
}

SemanticAnalyzer::SemanticAnalyzer(string input):
lexer(input)
{
	labelp = 0;
	datap = 0;
	string::iterator it = input.end();
	while(it != input.begin() && *it != '.') it--;
	codeOut = string(input.begin(),it);
	codeOut.push_back('.');
	codeOut.push_back('s');
}

SemanticAnalyzer::SemanticAnalyzer(string input, string output):
lexer(input),
codeOut(output)
{
    labelp = 0;
    datap = 0;
}

SemanticAnalyzer::~SemanticAnalyzer(){}

void SemanticAnalyzer::analyze()
{
	try
	{
		fout.open(codeOut);
    	//cout<<codeOut<<endl;
		if(!fout.is_open()) throw 10;
		get(token);
		program();
		if(!match(_OVER)) throw 11;
		cout<<"编译成功！"<<endl;
		fout.close();
	}
	catch(int err)
	{
		switch(err)
		{
			case 1:cout<<"缺少{！"<<endl;break;
			case 2:cout<<"缺少}！"<<endl;break;
			case 3:cout<<"缺少标识符！"<<endl;break;
			case 4:cout<<"少分号！"<<endl;break;
			case 5:cout<<"缺少（！"<<endl;break;
			case 6:cout<<"缺少）！"<<endl;break;
			case 7:cout<<"缺少操作数！"<<endl;break;
			case 8:cout<<"没有主函数入口！"<<endl;break;
            case 9:cout<<"错误的语句"<<endl;break;
            case 10:cout<<"创建输出文件失败！"<<endl;break;
            case 11:cout<<"代码非正常结束"<<endl;break;
			case 22:cout<<"变量重复定义！"<<endl;break;
			case 23:cout<<"变量未声明！"<<endl;break;
		}
	}
}


void SemanticAnalyzer::print_vartable()
{
	cout<<"\t符号表"<<endl;
	cout<<"\t名字\t地址"<<endl;
	for(int i = 0;i < varTable.size();i++)
	{
		cout<<"\t"<<varTable[i].name<<'\t'<<varTable[i].address<<endl;
	}
}

void SemanticAnalyzer::get(Token &tk)
{
	tk = lexer.next();
	//cout<<token.term<<token.value<<endl;
}

void SemanticAnalyzer::name_def(string &name)
{
    for(unsigned long i = 0 ;i < varTable.size(); i++)
	{
		if(varTable[i].name == name)
		{
			throw 22;
		}
	}
	varTable.push_back(Record(name,datap));
	datap++;
}

void SemanticAnalyzer::lookup(string &name,int &address)
{
	for(int i = 0; i < varTable.size(); i++)
	{
		if(name == varTable[i].name)
		{
			address = varTable[i].address;
			return;
		}
	}
	throw 23;
}


void SemanticAnalyzer::program()
{
	if(!match(_MAIN)) throw 8;
	get(token);
	if(!match(_PARENTHESE_L)) throw 5;
	get(token);
	if(!match(_PARENTHESE_R)) throw 6;
	get(token);
	if(!match(_BRACE_L)) throw 1;
	get(token);
	declaration_list();
	statement_list();
	if(!match(_BRACE_R)) throw 2;
	stop();
    get(token);
}

void SemanticAnalyzer::compound_stat()
{
	get(token);
	statement_list();
	if(!match(_BRACE_R)) throw 2;
	get(token);
}

void SemanticAnalyzer::statement()
{
	if(match(_IF)) if_stat();
	else if(match(_WHILE)) while_stat();
	else if(match(_FOR)) for_stat();
	else if(match(_READ)) read_stat();
	else if(match(_PRINT)) print_stat();
	else if(match(_BRACE_L)) compound_stat();
	else if(match(_ID) || match(_NUM) || match(_PARENTHESE_L)) expression_stat();
	else throw 9;
}

void SemanticAnalyzer::expression_stat()
{
	if(match(_SEMICOLON))
	{
		get(token);
		return;
	}
	expression();
	pop();
	if(match(_SEMICOLON))
	{
		get(token);
		return;
	}
	else throw 4;
}

void SemanticAnalyzer::expression()
{
	if(match(_ID))
	{
		Token tk;
		get(tk);
		if(tk.term == _ASSIGN) //由于有左公共因子，需要向前看一步
		{
			int address;
			lookup(token.value,address);
			get(token);
			bool_expr();
			store(address);
		}
		else
		{
			lexer.last();
			bool_expr();
		}
	}
	else bool_expr();
}

void SemanticAnalyzer::bool_expr()
{
	additive_expr();
	if(match(_BIGGER)||match(_BIGGEROREQUAL)||
		match(_SMALLLER) ||match(_SIMMALLEROREQUAL)||
		match(_EQUAL)||match(_NOTEQUAL))
	{
		Term op = token.term;
		get(token);
		additive_expr();
		operate(op);
	}
}

void SemanticAnalyzer::additive_expr()
{
	term();
	while(match(_ADD) || match(_SUB))
	{
		Term op = token.term;
		get(token);
		term();
		operate(op);
	}
}

void SemanticAnalyzer::term()
{
	factor();
	while(match(_MUL) || match(_DIV))
	{
		Term op = token.term;
		get(token);
		factor();
		operate(op);
	}
}

void SemanticAnalyzer::factor()
{
	if(match(_PARENTHESE_L))
	{
		get(token);
		expression();
		if(!match(_PARENTHESE_R)) throw 6;
		get(token);
	}
	else if(match(_ID))
	{
		int address;
		lookup(token.value,address);
		load(address);
		get(token);
	}
	else if(match(_NUM))
	{
		loadi(token.value);
		get(token);
	}
	else throw 7;
}

void SemanticAnalyzer::if_stat()
{
	get(token);
	if(!match(_PARENTHESE_L)) throw 5;
	get(token);
	expression();
	if(!match(_PARENTHESE_R)) throw 6;
	int label1 = labelp++;
	brf(label1);
	get(token);
	statement();
	int label2 = labelp++;
	br(label2);
	set_label(label1);
	if(match(_ELSE))
	{
		get(token);
		statement();
	}
	set_label(label2);
}

void SemanticAnalyzer::while_stat()
{
	int label1 = labelp++;
	set_label(label1);
	get(token);
	if(!match(_PARENTHESE_L)) throw 5;
	get(token);
	expression();
	if(!match(_PARENTHESE_R)) throw 6;
	int label2 = labelp++;
	brf(label2);
	get(token);
	statement();
	br(label1);
	set_label(label2);
}

void SemanticAnalyzer::for_stat()
{
	int label1,label2,label3,label4;
	get(token);
	if(!match(_PARENTHESE_L)) throw 5;

	get(token);
	expression();
	pop();
	if(!match(_SEMICOLON)) throw 4;
	
	label1 = labelp++;
	set_label(label1);//set label1 for loop condition
	get(token);
	expression();
	label2 = labelp++;
	brf(label2);//false transfer->end
	label3 = labelp++;
	br(label3);//->loop body
	if(!match(_SEMICOLON)) throw 4;

	label4 = labelp++;
	set_label(label4);
	get(token);
	expression();
	pop();
	br(label1);
	if(!match(_PARENTHESE_R)) throw 6;

	set_label(label3);
	get(token);
	statement();
	br(label4);
	set_label(label2);
}

/* <print_stat> := print<expression>@OUT 
OUT指令将栈顶元素输出*/

void SemanticAnalyzer::print_stat()
{
	get(token);
	expression();
	if(token.term != _SEMICOLON) throw 4;
	out();
	get(token);
}

void SemanticAnalyzer::read_stat()
{
	int address;
	get(token);
	if(!match(_ID)) throw 3;
	lookup(token.value,address);
	in();
	store(address);
	pop();
	get(token);
	if(!match(_SEMICOLON)) throw 4;
	get(token);
}

void SemanticAnalyzer::declaration_stat()
{
	get(token);
	if(!match(_ID)) throw 3;
	name_def(token.value);
	get(token);
	if(!match(_SEMICOLON)) throw 4;
	get(token);
}

void SemanticAnalyzer::declaration_list()
{
	while(match(_INT))
	{
		declaration_stat();
	}
}

void SemanticAnalyzer::statement_list()
{
	while(!match(_BRACE_R))
	{
		statement();
	}
}

inline void SemanticAnalyzer::stop()
{
	fout<<"\tSTOP"<<endl;
}

inline void SemanticAnalyzer::pop()
{
	fout<<"\tPOP"<<endl;
}

inline void SemanticAnalyzer::br(int label)
{
	fout<<"\tBR "<<label<<endl;
}

inline void SemanticAnalyzer::brf(int label)
{
	fout<<"\tBRF "<<label<<endl;
}

inline void SemanticAnalyzer::operate(Term op)
{
	if(op == _BIGGER) fout<<"\tGT"<<endl;
	if(op == _BIGGEROREQUAL) fout<<"\tGE"<<endl;
	if(op == _SMALLLER) fout<<"\tLES"<<endl;
	if(op == _SIMMALLEROREQUAL) fout<<"	LE"<<endl;
	if(op == _EQUAL) fout<<"\tEQ"<<endl;
	if(op == _NOTEQUAL) fout<<"\tNOTEQ"<<endl;
	if(op == _ADD) fout<<"\tADD"<<endl;
	if(op == _SUB) fout<<"\tSUB"<<endl;
	if(op == _MUL) fout<<"\tMULT"<<endl;
	if(op == _DIV) fout<<"\tDIV"<<endl;
}

inline void SemanticAnalyzer::store(int address)
{
	fout<<"\tSTO "<<address<<endl;
}

inline void SemanticAnalyzer::set_label(int label)
{
	fout<<"LABEL"<<label<<":"<<endl;
}

inline void SemanticAnalyzer::load(int address)
{
	fout<<"\tLOAD "<<address<<endl;
}

inline void SemanticAnalyzer::loadi(string constnum)
{
	fout<<"\tLOADI "<<constnum<<endl;
}

inline void SemanticAnalyzer::in()
{
	fout<<"\tIN"<<endl;
}
inline void SemanticAnalyzer::out()
{
	fout<<"\tOUT"<<endl;
}
