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

SemanticAnalyzer::SemanticAnalyzer(const char* input):lexer(input)
{
	labelp = 0;
	datap = 0;
	tokenStream = input;
	string::iterator it = tokenStream.begin();
	while(it != tokenStream.end() && *it != '.') it++;
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
	if(!fout.is_open())
	{
		cout<<"\n"<<"创建"<<codeOut<<"错误"<<endl;
		es = 10;
		return(es);
	}

	if(es == 0)	es = program();

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
	}
	fin.close();
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

void SemanticAnalyzer::get(lexicalAnalyzer::Token &tk)
{
	//fin>>token.term>>token.value;
	tk = lexer.next();
	cout<<token.term<<token.value<<endl;
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
	get(token);
	if(token.term != _BRACE_L)
	{
		es = 1;
		return(es);
	}
	get(token);
	es = declaration_list();
	if(es > 0)	return(es);

	print_vartable();

	es = statement_list();
	if(es > 0)	return(es);
	if(token.term != _BRACE_R)
	{
		es = 2;
		return(es);
	}
	fout<<"\tSTOP"<<endl;
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

int SemanticAnalyzer::expression_stat(){
	int es = 0;
	if(token.term == _SEMICOLON )
	{
		get(token);
		return(es);
	}
	es = expression();
	if(es > 0) return(es);
	fout<<"\tPOP"<<endl;
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
    lexicalAnalyzer::Token tk;
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
			fout<<"\tSTO "<<address<<endl;
		}
		else
		{
			//fin.seekg(fileadd,std::ios::beg);
			//cout<<token.term<<token.value<<endl;
			lexer.back();
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
		lexicalAnalyzer::Term op = token.term;
		get(token);
		es = additive_expr();
		if(es > 0) return(es);
		if(op == _BIGGER) fout<<"\tGT"<<endl;
		if(op == _BIGGEROREQUAL) fout<<"\tGE"<<endl;
		if(op == _SMALLLER) fout<<"\tLES"<<endl;
		if(op == _SIMMALLEROREQUAL) fout<<"	LE"<<endl;
		if(op == _EQUAL) fout<<"\tEQ"<<endl;
		if(op == _NOTEQUAL) fout<<"\tNOTEQ"<<endl;
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
		lexicalAnalyzer::Term op = token.term;
		get(token);
		es = term();
		if(es > 0) return(es);
		if(op == _ADD) fout<<"\tADD"<<endl;
		if(op == _SUB) fout<<"\tSUB"<<endl;
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
		lexicalAnalyzer::Term op = token.term;
		get(token);
		es = factor();
		if(es > 0) return(es);
		if(op == _MUL) fout<<"\tMULT"<<endl;
		if(op == _DIV) fout<<"\tDIV"<<endl;
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
			fout<<"\tLOAD "<<address<<endl;
			get(token);
			return(es);
		}
		if(token.term == _NUM)
		{
			fout<<"\tLOADI "<<token.value<<endl;
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
	fout<<"\tBRF "<<label1<<endl;
	get(token);
	es = statement();
	if(es > 0) return(es);
	label2 = labelp++;
	fout<<"\tBE LABEL"<<label2<<endl;
	fout<<"LABEL"<<label1<<":"<<endl;
	if(token.term == "else")
	{
		get(token);
		es = statement();
		if(es > 0) return(es);
	}
	fout<<"LABEL"<<label2<<":"<<endl;
	return(es);
}

int SemanticAnalyzer::while_stat()
{
	int es = 0,label1,label2;
	label1 = labelp++;
	fout<<"LABEL"<<label1<<":"<<endl;
	get(token);
	if(token.term != _PARENTHESE_L) return(es = 5);
	get(token);
	es = expression();
	if(es > 0) return(es);
	if(token.term != _PARENTHESE_R) return(es = 6);
	label2 = labelp++;
	fout<<"\tBRF "<<label2<<endl;
	get(token);
	es = statement();
	if(es > 0) return(es);
	fout<<"\tBR "<<label1<<endl;
	fout<<"LABEL"<<label2<<":"<<endl;
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
	fout<<"\tPOP"<<endl;
	if(token.term != _SEMICOLON) return(es = 4);
	
	label1 = labelp++;
	fout<<"LABEL"<<label1<<":"<<endl;//set label1 for loop condition
	get(token);
	es = expression();
	if(es > 0) return(es);
	label2 = labelp++;
	fout<<"\tBRF "<<label2<<endl;//false transfer->end
	label3 = labelp++;
	fout<<"\tBR "<<label3<<endl;//->loop body
	if(token.term != _SEMICOLON) return(es = 4);

	label4 = labelp++;
	fout<<"LABEL"<<label4<<":"<<endl;
	get(token);
	es = expression();
	if(es > 0)	return(es);
	fout<<"\tPOP"<<endl;
	fout<<"\tBR "<<label1<<endl;
	if(token.term != _PARENTHESE_R) return(es = 6);

	fout<< "LABEL"<<label3<<":"<<endl;
	get(token);
	es = statement();
	if(es > 0) return(es);
	fout<<"\tBR "<<label4<<endl;
	fout<<"LABEL"<<label2<<":"<<endl;
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
	fout<<"\tOUT"<<endl;
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
	fout<<"\tIN"<<endl;
	fout<<"\tSTO "<<address<<endl;
	fout<<"\tPOP"<<endl;
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
