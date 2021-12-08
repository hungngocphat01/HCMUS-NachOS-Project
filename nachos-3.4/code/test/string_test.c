#include "syscall.h"

int main() {
    char ten[100];
    PrintString("Ten ban la gi? >> ");
    ReadString(ten, 100);
    PrintString("Xin chao, ");
    PrintString(ten);
    PrintString("\n");
}
