#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "virtualMachine.h"

#define _DEBUG


const int textSize = 1024;
const int dataSize = 1024;
const int stackSize = 1024;

virtualMachine:: virtualMachine()
{
	pc = textSegment =  new int[textSize];
	sp =  stackSegment = new int[dataSize];
	sp += sizeof(int)*(dataSize);
}
virtualMachine:: ~virtualMachine()
{
	delete [] textSegment;
	delete [] stackSegment;
}

int virtualMachine:: loadInstruction(list<int> & instructionStream)
{
	if (instructionStream.size() > textSize)
	{
		cout << "text size is too short!" << endl;
		return -1;
	}
	int* textPointer = pc;
	for (list<int>::iterator iter = instructionStream.begin(); iter != instructionStream.end(); iter++)
	{
		*textPointer++ = *iter;
#ifdef _DEBUG
		cout << *iter << endl;
#endif
	}
	return 0;
}

int virtualMachine:: run(list<int> &instructionStream)
{
	loadInstruction(instructionStream);
	cout<<"load finish"<<endl;
	int op, *tmp;
	while (1)
	{
		op = *pc++; // get next operation code

		if (op == IMM)
		{
			ax = *pc++;   // load immediate value to ax
		}
		 else if (op == LI)
		 {
		 	ax = *(int *)ax;   // load integer to ax, address in ax
		 }
		// else if (op == SC)
		// {
		// 	ax = *(char *)*sp++ = ax;   // save character to address, value in ax, address on stack
		// }
		 else if (op == SI)
		 {
		 	*(int *)*sp++ = ax;   // save integer to address, value in ax, address on stack
		 }
		else if (op == PUSH)
		{
			*--sp = ax;   // push the value of ax onto the stack
		}
		// else if (op == JMP)
		// {
		// 	pc = (int *)*pc;   // jump to the address
		// }
		// else if (op == JZ)
		// {
		// 	pc = ax ? pc + 1 : (int *)*pc;   // jump if ax is zero
		// }
		// else if (op == JNZ)
		// {
		// 	pc = ax ? (int *)*pc : pc + 1;   // jump if ax is zero
		// }
		// else if (op == CALL)
		// {
		// 	*--sp = (int)(pc + 1);    // call subroutine
		// 	pc = (int *)*pc;
		// }
		// //else if (op == RET)  {pc = (int *)*sp++;}                              // return from subroutine;
		// else if (op == ENT)
		// {
		// 	*--sp = (int)bp;    // make new stack frame
		// 	bp = sp;
		// 	sp = sp - *pc++;
		// }
		// else if (op == ADJ)
		// {
		// 	sp = sp + *pc++;   // add esp, <size>
		// }
		// else if (op == LEV)
		// {
		// 	sp = bp;    // restore call frame and PC
		// 	bp = (int *)*sp++;
		// 	pc = (int *)*sp++;
		// }
		// else if (op == ENT)
		// {
		// 	*--sp = (int)bp;    // make new stack frame
		// 	bp = sp;
		// 	sp = sp - *pc++;
		// }
		// else if (op == ADJ)
		// {
		// 	sp = sp + *pc++;   // add esp, <size>
		// }
		// else if (op == LEV)
		// {
		// 	sp = bp;    // restore call frame and PC
		// 	bp = (int *)*sp++;
		// 	pc = (int *)*sp++;
		// }
		// else if (op == LEA)
		// {
		// 	ax = (int)(bp + *pc++);   // load address for arguments.
		// }

		else if (op == OR)  ax = *sp++ | ax;
		else if (op == XOR) ax = *sp++ ^ ax;
		else if (op == AND) ax = *sp++ & ax;
		else if (op == EQ)  ax = *sp++ == ax;
		else if (op == NE)  ax = *sp++ != ax;
		else if (op == LT)  ax = *sp++ < ax;
		else if (op == LE)  ax = *sp++ <= ax;
		else if (op == GT)  ax = *sp++ >  ax;
		else if (op == GE)  ax = *sp++ >= ax;
		else if (op == SHL) ax = *sp++ << ax;
		else if (op == SHR) ax = *sp++ >> ax;
		else if (op == ADD) ax = *sp++ + ax;
		else if (op == SUB) ax = *sp++ - ax;
		else if (op == MUL) ax = *sp++ * ax;
		else if (op == DIV) ax = *sp++ / ax;
		else if (op == MOD) ax = *sp++ % ax;


		else if (op == EXIT)
		{
			cout<<"exit:"<<*sp<<endl;
			return 0;
			
			// return *sp;
		}
		// else if (op == OPEN)
		// {
		// 	ax = open((char *)sp[1], sp[0]);
		// }
		// else if (op == CLOS)
		// {
		// 	ax = close(*sp);
		// }
		// else if (op == READ)
		// {
		// 	ax = read(sp[2], (char *)sp[1], *sp);
		// }
		// else if (op == PRTF)
		// {
		// 	tmp = sp + pc[1];
		// 	ax = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]);
		// }
		// else if (op == MALC)
		// {
		// 	ax = (int)malloc(*sp);
		// }
		// else if (op == MSET)
		// {
		// 	ax = (int)memset((char *)sp[2], sp[1], *sp);
		// }
		// else if (op == MCMP)
		// {
		// 	ax = memcmp((char *)sp[2], (char *)sp[1], *sp);
		// }
		else
		{
			// printf("unknown instruction:%d\n", op);
			return -1;
		}
	}
	return 0;
}



int main(int argc, char **argv)
{

	list<int> inst;
	inst.push_back(IMM);
	inst.push_back(3);
	inst.push_back(PUSH);
	inst.push_back(IMM);
	inst.push_back(4);
	inst.push_back(ADD);
	inst.push_back(PUSH);
	inst.push_back(EXIT);

	virtualMachine VM;
	// int res = VM.loadInstruction(inst);
	VM.run(inst);
	cout<<sizeof(int)<<","<<sizeof(int*)<<endl;

}
