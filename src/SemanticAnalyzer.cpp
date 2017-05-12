#include <cstdio>
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


SemanticAnalyzer::SemanticAnalyzer(string &input):lexer(input)
{
	labelp = 0;
	datap = 0;
	tokenStream = input;
	string::iterator it = tokenStream.end();
	while(it != tokenStream.begin() && *it != '.') it--;
	codeOut = string(tokenStream.begin(),it);
	codeOut.push_back('.');
	codeOut.push_back('s');
}

SemanticAnalyzer::~SemanticAnalyzer(){}

int SemanticAnalyzer::analyze()
{
	int es = 0;
	/*fin.open(tokenStream);
	if(!fin.is_open())
	{
		cout<<"\n"<<"打开"<<tokenStream<<"错误"<<endl;
		es = 10;
		return(es);
	}*/
	fout.open(codeOut);
    //cout<<codeOut<<endl;
	if(!fout.is_open())
	{
		cout<<"\n"<<"创建"<<codeOut<<"错误"<<endl;
		es = 10;
		return(es);
	}

	if(es == 0)
	{
		get(token);
		es = program();
	}

	printf("==语法语义分析及代码生成程序结果==\n");
	switch(es)
	{
		case 0:printf("语法语义分析成功并抽象机汇编生成代码！\n");break;
		case 10:printf("打开文件失败！\n");break;
		case 1:printf("缺少{！\n");break;
		case 2:printf("缺少}！\n");break;
		case 3:printf("缺少标识符！\n");break;
		case 4:printf("少分号！\n");break;
		case 5:printf("缺少（！\n");break;
		case 6:printf("缺少）！\n");break;
		case 7:printf("缺少操作数！\n");break;
		case 21:printf("符号表溢出！\n");break;
		case 22:printf("变量重复定义！\n");break;
		case 23:printf("变量未声明！\n");break;
		case 30:printf("没有主函数入口！\n");break;
	}
	//fin.close();
	fout.close();
	return(es);
}


void SemanticAnalyzer::print_vartable()
{
	printf("\t符号表\n");
	printf("\t名字\t地址\n");
	for(int i = 0;i < varTable.size();i++)
	{
		cout<<"\t"<<varTable[i].name<<'\t'<<varTable[i].address<<endl;
	}
}

void SemanticAnalyzer::get(Token &tk)
{
	//fin>>token.term>>token.value;
	tk = lexer.next();
	//cout<<token.term<<token.value<<endl;
}

int SemanticAnalyzer::name_def(string &name)
{
	int es = 0;
    for(unsigned long i = 0 ;i < varTable.size(); i++)
	{
		if(varTable[i].name == name)
		{
			es = 22;
			break;
		}
	}
	if(es > 0)	return(es);
	varTable.push_back(Record(name,datap));
	datap++;
	return(es);
}

int SemanticAnalyzer::lookup(string &name,int &address)
{
	int es = 0;
	for(int i = 0; i < varTable.size(); i++)
	{
		if(name == varTable[i].name)
		{
			address = varTable[i].address;
			return es;
		}
	}
	es = 23;
	return(es);
}


int SemanticAnalyzer::program()
{
	int es = 0;
	if(token.term != _MAIN) return (es = 30);
	get(token);
	if(token.term != _PARENTHESE_L) return (es = 5);
	get(token);
	if(token.term != _PARENTHESE_R) return (es =6);
	get(token);
	if(token.term != _BRACE_L)
	{
		es = 1;
		return(es);
	}
	get(token);
	es = declaration_list();
	if(es > 0)	return(es);

	//print_vartable();

	es = statement_list();
	if(es > 0)	return(es);
	if(token.term != _BRACE_R)
	{
		es = 2;
		return(es);
	}
	stop();
	return(es);
}

int SemanticAnalyzer::compound_stat()
{
	int es = 0;
	get(token);
	es = statement_list();
	return(es);
}

int SemanticAnalyzer::statement()
{
	int	es = 0;
	if(es ==0 && token.term == _IF)
		es = if_stat();
	if(es ==0 && token.term == _WHILE)
		es = while_stat();
	if(es ==0 && token.term == _FOR)
		es = for_stat();
	if(es ==0 && token.term == _READ)
		es = read_stat();
	if(es ==0 && token.term == _PRINT)
		es = print_stat();
	if(es ==0 && token.term == _BRACE_L)
		es = compound_stat();
	if(es == 0 && (token.term == _ID || token.term == _NUM || token.term == _PARENTHESE_L))
		es = expression_stat();
	return(es);
}

int SemanticAnalyzer::expression_stat()
{
	int es = 0;
	if(token.term == _SEMICOLON )
	{
		get(token);
		return(es);
	}
	es = expression();
	if(es > 0) return(es);
	pop();
	if(token.term == _SEMICOLON)
	{
		get(token);
		return(es);
	}
	else
	{
		es = 4;
		return(es);
	}
}

int SemanticAnalyzer::expression()
{
    int es = 0;
    //std::streamoff fileadd;
    Token tk;
	if(token.term == _ID)
	{
		//fileadd = fin.tellg();
		get(tk);
		if(tk.term == _ASSIGN) //由于有左公共因子，需要向前看一步
		{
			int address;
			es = lookup(token.value,address);
			if(es > 0) return(es);
			get(token);
			es = bool_expr();
			if(es > 0) return(es);
			store(address);
		}
		else
		{
			//fin.seekg(fileadd,std::ios::beg);
			//cout<<token.term<<token.value<<endl;
			lexer.last();
			es = bool_expr();
			if(es > 0) return(es);
		}
	}
	else
	{
		es = bool_expr();
	}
	return(es);
}

int SemanticAnalyzer::bool_expr()
{
	int es = 0;
	es = additive_expr();
	if(es > 0)
		return(es);
	if(token.term == _BIGGER||token.term == _BIGGEROREQUAL ||
		token.term == _SMALLLER ||token.term == _SIMMALLEROREQUAL||
		token.term == _EQUAL||token.term == _NOTEQUAL)
	{
		Term op = token.term;
		get(token);
		es = additive_expr();
		if(es > 0) return(es);
		operate(op);
	}
	return(es);
}

int SemanticAnalyzer::additive_expr(){
	int es = 0;
	es = term();
	if(es > 0)
		return(es);
	while(token.term == _ADD || token.term == _SUB)
	{
		Term op = token.term;
		get(token);
		es = term();
		if(es > 0) return(es);
		operate(op);
	}
	return(es);
}

int SemanticAnalyzer::term()
{
	int es = 0;
	es = factor();
	if(es > 0) return(es);
	while(token.term == _MUL || token.term == _DIV)
	{
		Term op = token.term;
		get(token);
		es = factor();
		if(es > 0) return(es);
		operate(op);
	}
	return(es);
}

int SemanticAnalyzer::factor()
{
	int es = 0;
	if(token.term == _PARENTHESE_L)
	{
		get(token);
		es = expression();
		if(es > 0) return(es);
		if(token.term != _PARENTHESE_R) return(es = 6);
		get(token);
	}
	else
	{
		if(token.term == _ID)
		{
			int address;
			es = lookup(token.value,address);
			if(es > 0) return(es);
			load(address);
			get(token);
			return(es);
		}
		if(token.term == _NUM)
		{
			loadi(token.value);
			get(token);
			return(es);
		}
		else
		{
			es = 7;
			return(es);
		}
	}
	return(es);
}

int SemanticAnalyzer::if_stat()
{
	int es = 0,label1,label2;
	get(token);
	if(token.term != _PARENTHESE_L) return(es = 5);
	get(token);
	es = expression();
	if(es > 0) return(es);
	if(token.term != _PARENTHESE_R) return(es = 6);
	label1 = labelp++;
	brf(label1);
	get(token);
	es = statement();
	if(es > 0) return(es);
	label2 = labelp++;
	br(label2);
	set_label(label1);
	if(token.term == _ELSE)
	{
		get(token);
		es = statement();
		if(es > 0) return(es);
	}
	set_label(label2);
	return(es);
}

int SemanticAnalyzer::while_stat()
{
	int es = 0,label1,label2;
	label1 = labelp++;
	set_label(label1);
	get(token);
	if(token.term != _PARENTHESE_L) return(es = 5);
	get(token);
	es = expression();
	if(es > 0) return(es);
	if(token.term != _PARENTHESE_R) return(es = 6);
	label2 = labelp++;
	brf(label2);
	get(token);
	es = statement();
	if(es > 0) return(es);
	br(label1);
	set_label(label2);
	return(es);
}

int SemanticAnalyzer::for_stat()
{
	int es = 0,label1,label2,label3,label4;
	get(token);
	if(token.term != _PARENTHESE_L) return(es = 5);

	get(token);
	es = expression();
	if(es > 0) return(es);
	pop();
	if(token.term != _SEMICOLON) return(es = 4);
	
	label1 = labelp++;
	set_label(label1);//set label1 for loop condition
	get(token);
	es = expression();
	if(es > 0) return(es);
	label2 = labelp++;
	brf(label2);//false transfer->end
	label3 = labelp++;
	br(label3);//->loop body
	if(token.term != _SEMICOLON) return(es = 4);

	label4 = labelp++;
	set_label(label4);
	get(token);
	es = expression();
	if(es > 0)	return(es);
	pop();
	br(label1);
	if(token.term != _PARENTHESE_R) return(es = 6);

	set_label(label3);
	get(token);
	es = statement();
	if(es > 0) return(es);
	br(label4);
	set_label(label2);
	return(es);
}

/* <print_stat> := print<expression>@OUT 
OUT指令将栈顶元素输出*/

int SemanticAnalyzer::print_stat()
{
	int es = 0;
	get(token);
	es = expression();
	if(es > 0) return(es);
	if(token.term != _SEMICOLON) return(es = 4);
	out();
	get(token);
	return(es);
}

int SemanticAnalyzer::read_stat()
{
	int es = 0,address;
	get(token);
	if(token.term != _ID) return(es = 3);
	es = lookup(token.value,address);
	if(es > 0) return(es);
	in();
	store(address);
	pop();
	get(token);
	if(token.term != _SEMICOLON) return(es = 4);
	get(token);
	return(es);
}


int SemanticAnalyzer::declaration_stat()
{
	int es = 0;
	get(token);
	if(token.term != _ID) return(es = 3);
	es = name_def(token.value);
	if(es > 0) return(es);
	get(token);
	if(token.term != _SEMICOLON) return(es = 4);
	get(token);
	return(es);
}

int SemanticAnalyzer::declaration_list()
{
	int es = 0;
	while(token.term == _INT)
	{
		es = declaration_stat();
		if(es > 0)return(es);
	}
	return(es);
}

int SemanticAnalyzer::statement_list()
{
	int es = 0;
	while(token.term != _BRACE_R)
	{
		es = statement();
		if(es > 0)return(es);
	}
	return(es);
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
