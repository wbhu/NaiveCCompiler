/**
 * 
 * @author:		胡文博
 * @email:		huwenbo@mail.dlut.edu.cn
 * @dateTime:		2017-05-10 16:33:22
 * @description: 	
 */

#include <iostream>
#include <list>
#include<string>
using namespace std;

class simpleVitualMachine
{
    list<string> code;
    list<int> data;
    list<int> label;
public:
	simpleVitualMachine();
	~simpleVitualMachine();
    bool readAsmFile( string fileName);

};
