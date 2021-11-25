// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "syscall_utils.h"
#define MAX_SIZE 100

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

// Ham xy ly syscall Sub
void syscallSub() {
    int a = machine->ReadRegister(4);
    int b = machine->ReadRegister(5);
    int result = a - b;
    machine->WriteRegister(2, result);

    printf("Service called. %d - %d is %d\n", a, b, result);
}

// Ham xy ly syscall ReadChar
void syscallReadChar() {
    char c;
    // Doc 1 byte tu console
    int numBytesRead = synchconsole->Read(&c, 1);
    // synchconsole tra ve -1 => nguoi dung endstream
    if (numBytesRead < 0) {
        printf("\nRead cancelled.\n");
    }

    machine->WriteRegister(2, c);
}

// Ham xy ly syscall PrintChar
void syscallPrintChar() {
    // Doc tham so 1
    int arg = machine->ReadRegister(4);
    // Kiem tra tinh hop le cua ky tu
    if (arg < 0 || arg > 255) {
        printf("\nInvalid character!!! ASCII code: %d\n", arg);
        return;
    }

    char c = (char)arg;
    // Ghi 1 byte ra console
    synchconsole->Write(&c, 1);    
}

// Ham xu ly syscall ReadInt
void syscallReadInt() {
    // Tao mang rong co MAX_SIZE phan tu \0
    char* buffer = new char[MAX_SIZE];
    // Kich thuoc thuc su da doc
    int numBytesRead = synchconsole->Read(buffer, MAX_SIZE);
    // printf("\nBuffer read: %s\n", buffer);
    // printf("\nBytes read: %d\n", numBytesRead);

    // synchconsole tra ve -1 => nguoi dung end stream
    if (numBytesRead == -1) {
        printf("\nRead cancelled.\n");
        return;
    }

    bool isNegative = false;

    // Kiem tra so am
    if (buffer[0] == '-') {
        isNegative = true;
        // Bo ki tu dau tien di 
        numBytesRead--;
        buffer++;
    }

    int result = 0;
    // Thuat toan doi chuoi sang so nguyen
    // numBytesRead chinh la so chu so cua so vua nhap
    for (int i = 0; i < numBytesRead; i++) {
        // Kiem tra tinh hop le cua ki tu
        if (buffer[i] < '0' || buffer[i] > '9') {
            printf("\nInvalid digit: %c\n", buffer[i]);

            // Restore buffer
            if (isNegative) {
                buffer--;
            }
            delete[] buffer;
            return;
        }

        int k = buffer[i] - '0';
        result += k * pow(10, numBytesRead - i - 1);
    }
    if (isNegative) {
        result *= -1;
        buffer--;
    }

    // Tra ket qua ve
    machine->WriteRegister(2, result);

    delete[] buffer;
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    if (which == SyscallException) {
    	switch (type) {
    		// Khi nguoi dung goi syscall halt
    		case SC_Halt:
    			DEBUG('a', "Shutdown, initiated by user program.\n");
	   			interrupt->Halt();
	   			break; 
            // Tru 2 so 
            case SC_Sub:
                syscallSub();
                break;

            case SC_ReadChar:
                syscallReadChar();
                break;

            case SC_PrintChar:
                syscallPrintChar();
                break;

            case SC_ReadInt:
                syscallReadInt();
                break;
        }
        fetchNextInstruction();
    } else {
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);
    }
}
