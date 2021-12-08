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
// Macro de doc/ghi thanh ghi
#define SYSCALL_RET(VALUE) machine->WriteRegister(2, VALUE)
#define WRITE_REGISTER(REG, VALUE) machine->ReadRegister(REG, VALUE)
#define READ_REGISTER(REG) machine->ReadRegister(REG)
#define GET_ARGUMENT(N) machine->ReadRegister(N + 3)

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
    int a = GET_ARGUMENT(1);
    int b = GET_ARGUMENT(2);
    int result = a - b;
    SYSCALL_RET(result);

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

    SYSCALL_RET(c);
}

// Ham xy ly syscall PrintChar
void syscallPrintChar() {
    // Doc tham so 1
    int arg = GET_ARGUMENT(1);
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
    // debugging
    // printf("\nBuffer read: %s\n", buffer);
    // printf("\nBytes read: %d\n", numBytesRead);

    // synchconsole tra ve -1 => nguoi dung end stream
    if (numBytesRead == -1) {
        printf("\nRead cancelled.\n");
        delete[] buffer;
        return;
    }

    bool isNegative = false;
    int firstNumIndex = 0;      // Index cua ky tu dau tien (de xu ly cac dau cach va so am)
    
    for(firstNumIndex = 0; buffer[firstNumIndex] == ' ' && firstNumIndex < numBytesRead; firstNumIndex++);
    for(numBytesRead; buffer[numBytesRead - 1] == ' '; numBytesRead--);
    // firstNumIndex: can duoi, numBytesRead: can tren
    
    // printf("First num: %d, numBytesRead: %d\n", firstNumIndex, numBytesRead);
    // Kiem tra so am
    if (buffer[firstNumIndex] == '-') {
        isNegative = true;
        // Bo ki tu dau tien di
        firstNumIndex++;
    }

    int result = 0;
    // Thuat toan doi chuoi sang so nguyen
    // numBytesRead chinh la so chu so cua so vua nhap
    for (int i = firstNumIndex; i < numBytesRead; i++) {
        if (buffer[i] < '0' || buffer[i] > '9') {
            printf("\nInvalid digit: %c\n", buffer[i]);

            delete[] buffer;
            SYSCALL_RET(0);
            return;
        }

        int k = buffer[i] - '0';
        result += k * pow(10, numBytesRead - i - 1);
    }
    if (isNegative) {
        result *= -1;
    }
    // printf("Debug: Read %d\n", result);

    // Tra ket qua ve
    SYSCALL_RET(result);
    delete[] buffer;
}

// Ham xu ly syscall PrintInt
void syscallPrintInt()
{	
	int number = GET_ARGUMENT(1);
	if(number == 0)
    {
        synchconsole->Write("0", 1); // In ra man hinh so 0
        // IncreasePC();
        return;    
    }
                    
    /*Qua trinh chuyen so thanh chuoi de in ra man hinh*/
    bool isNegative = false; // gia su la so duong
    int numberOfNum = 0; // Bien de luu so chu so cua number
    int firstNumIndex = 0; 
			
    if(number < 0)
    {
        isNegative = true;
         number = number * -1; // Nham chuyen so am thanh so duong de tinh so chu so
        firstNumIndex = 1; 
    } 	
                    
    int t_number = number; // bien tam cho number
    while(t_number)
    {
        numberOfNum++;
        t_number /= 10;
    }
    
	// Tao buffer chuoi de in ra man hinh
    char* buffer;
    int MAX_BUFFER = 255;
    buffer = new char[MAX_BUFFER + 1];
    for(int i = firstNumIndex + numberOfNum - 1; i >= firstNumIndex; i--)
    {
        buffer[i] = (char)((number % 10) + 48);
        number /= 10;
    }
    if(isNegative)
    {
        buffer[0] = '-';
		buffer[numberOfNum + 1] = 0;
        synchconsole->Write(buffer, numberOfNum + 1);
        delete[] buffer;
        // IncreasePC();
        return;
    }
	buffer[numberOfNum] = 0;	
    synchconsole->Write(buffer, numberOfNum);
    delete[] buffer;
    // IncreasePC();
    return;        		
}

void syscallReadString() {
    int Addr;
    int length;
	char* buffer;
	Addr = GET_ARGUMENT(1); // Lay dia chi tham so buffer truyen vao tu thanh ghi so 4
	length = GET_ARGUMENT(2); // Lay do dai toi da cua chuoi nhap vao tu thanh ghi so 5
	buffer = new char[length + 1];
	int bytesRead = synchconsole->Read(buffer, length); // Goi ham Read cua SynchConsole de doc chuoi
	
    // Neu nguoi dung Ctrl-A: ket thuc doc
    if (bytesRead < 0) {
        delete[] buffer;
        printf("Read cancelled.");
        return;
    }

    System2User(Addr, length, buffer); // Copy chuoi tu vung nho System Space sang vung nho User Space
	delete[] buffer; 
	// IncreasePC(); // Tang Program Counter 
	return;
}

void syscallPrintString(){
    // Input: Buffer(char*)
	// Output: Chuoi doc duoc tu buffer(char*)
	// Cong dung: Xuat mot chuoi la tham so buffer truyen vao ra man hinh
	int Addr;
	char* buffer;
	Addr = GET_ARGUMENT(1); // Lay dia chi cua tham so buffer tu thanh ghi so 4
	// for(maxLength = 0; Addr[maxLength] != 0; maxLength++); SEGMENTATION FAULT

    buffer = User2System(Addr, 255); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai 255 ki tu
	int length = 0;
	while (buffer[length] != 0) length++; // Dem do dai that cua chuoi
	synchconsole->Write(buffer, length + 1); // Goi ham Write cua SynchConsole de in chuoi
	delete[] buffer; 
	    //IncreasePC(); // Tang Program Counter 
}
    

void
ExceptionHandler(ExceptionType which)
{
    int type = READ_REGISTER(2);
	
	switch(which){
		case NoException:
			return;
		
		case PageFaultException:
			DEBUG('a', "\nNo valid translation found.\n");
			printf("\nNo valid translation found.\n");
			interrupt->Halt();
		break;

		case ReadOnlyException:
			DEBUG('a', "\nWrite attempted to page marked read-only.\n");
			printf("\nWrite attempted to page marked read-only.\n");
			interrupt->Halt();
			break;

		case BusErrorException:
			DEBUG('a', "\nTranslation resulted invalid physical address.\n");
			printf("\nTranslation resulted invalid physical address.\n");
			interrupt->Halt();
			break;

		case AddressErrorException:
			DEBUG('a', "\nUnaligned reference or one that was beyond the end of the address space.\n");
			printf("\nUnaligned reference or one that was beyond the end of the address space.\n");
			interrupt->Halt();
			break;

		case OverflowException:
			DEBUG('a', "\nInteger overflow in add or sub.\n");
			printf("\nInteger overflow in add or sub.\n");
			interrupt->Halt();
			break;

		case IllegalInstrException:
			DEBUG('a', "\nUnimplemented or reserved instr.\n");
			printf("\nUnimplemented or reserved instr.\n");
			interrupt->Halt();
			break;

		case NumExceptionTypes:
			DEBUG('a', "\nNumber exception types.\n");
			printf("\nNumber exception types.\n");
			interrupt->Halt();
			break;
			
		case SyscallException:
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

				case SC_PrintInt:
					syscallPrintInt();
					break;

        		case SC_ReadString:
            		syscallReadString();
            		break;	

        		case SC_PrintString:
            		syscallPrintString();
					break;    
				default:
					break;
			}		
            // Tang program counter
			fetchNextInstruction();
	}
}
