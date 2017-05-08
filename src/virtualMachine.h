/**
 *
 * @author:		胡文博
 * @email:		huwenbo@mail.dlut.edu.cn
 * @dateTime:		2017-05-05 20:19:03
 * @description:
 */

#include <iostream>
#include <list>

using namespace std;
// instructions
enum { LEA , IMM , JMP , CALL, JZ  , JNZ , ENT , ADJ , LEV , LI  , LC  , SI  , SC  , PUSH,
       OR  , XOR , AND , EQ  , NE  , LT  , GT  , LE  , GE  , SHL , SHR , ADD , SUB , MUL , DIV , MOD ,
       OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
     };



class virtualMachine
{
private:
	int* textSegment;
	int* stackSegment;
	int *pc, *sp, ax; // virtual machine registers
	// list<int> instructionStream;
	int loadInstruction(list<int>& instructionStream);

public:
	virtualMachine();
	~virtualMachine();
	int run(list<int> &instructionStream );

};
