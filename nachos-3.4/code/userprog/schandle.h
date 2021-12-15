#ifndef SCHANDLE_H
#define SCHANDLE_H
#include "syscall_utils.h"

int doSC_Create();
int doSC_Exit();
int doSC_Close();
int doSC_Seek();
int doSC_Read();
int doSC_Write();
int doSC_Open();
#endif
