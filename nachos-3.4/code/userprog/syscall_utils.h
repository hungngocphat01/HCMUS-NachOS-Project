/**
	File dinh nghia cac ham tien ich duoc dung boi cac system call
**/
#include "system.h"

// https://github.com/ayusek/NachOS-Assignment--CS330/blob/master/assignment%201/assignment1.txt
void fetchNextInstruction() {
    // Tang thanh ghi program counter
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
}

// Ham tinh x^n
int pow(int x, int n) {
	int result = 1;
	for (int i = 0; i < n; i++) 
		result *= x;
	return result;
}