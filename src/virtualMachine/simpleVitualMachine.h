/**
 *
 * @author:		胡文博
 * @email:		huwenbo@mail.dlut.edu.cn
 * @dateTime:		2017-05-10 16:33:22
 * @description:
 */
#ifndef _SIMPLEVM_
#define _SIMPLEVM_

#include <list>
#include <string>
#include <vector>
#include <stack>

using namespace std;

class simpleVitualMachine
{
	vector<string> code;
	int pc;
	vector<int> data;
	vector<int> label;
	stack<int> calculateStack;
	void dispCode();
	void dispLabel();
	int readAsmFile( string fileName);
public:
	simpleVitualMachine();
	simpleVitualMachine(string fileName);
	~simpleVitualMachine();
	int run();

};
#endif