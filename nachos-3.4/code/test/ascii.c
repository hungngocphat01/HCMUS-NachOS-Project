#include "syscall.h"
int main()
{
    char i = 0;
	PrintString("-------BANG MA ASCII-------\n");
	for (; i < 127; i++) //i <= 127
	{
		PrintInt((int)i);
		PrintChar('\t');
		PrintChar(i);
		PrintChar('\n');
	}
}
