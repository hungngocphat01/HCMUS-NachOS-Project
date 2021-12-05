#include "syscall.h"

int main() {
	int a;
	int b;
	int c;
	PrintString("Nhap a: ");
	a = ReadInt();
	PrintString("Nhap b: ");
	b = ReadInt();

	c = a + b;
	PrintString("\nTong la: ");
	PrintInt(c);
}