/**
 * 
 * @author:		胡文博
 * @email:		huwenbo@mail.dlut.edu.cn
 * @dateTime:		2017-05-10 16:33:00
 * @description: 	
 */
#include <simpleVitualMachine.h>
#include <fstream>

using namespace std;

simpleVitualMachine::simpleVitualMachine()
{

}

simpleVitualMachine::~simpleVitualMachine()
{

}


bool simpleVitualMachine::readAsmFile(string fileName)
{
    fstream sourceFile;
    sourceFile.open(fileName);
    string s;
    while(sourceFile>>s)
    {
#ifdef _DEBUG
        cout<<""<<s<<endl;
    }
}
