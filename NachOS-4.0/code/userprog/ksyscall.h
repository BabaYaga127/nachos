/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "main.h"
#include "synchconsole.h"

#define MAX_INT_LENGTH 12
char _num_buf[MAX_INT_LENGTH+1];

#define MAX_STRING_LENGTH 255
char _str_buf[MAX_STRING_LENGTH+1];

void _CaseHalt();
void _CaseAdd();
void _CaseReadNum();
void _CasePrintNum();
void _CaseReadChar();
void SysPrintChar(char);
void _CasePrintChar();
void _CaseRandomNum();
void _CaseReadString();
void SysPrintString(char *buf);
void _CasePrintString();
void _IncreaseProgramCounter();
char* User2System(int virtAddr,int limit);
int System2User(int virtAddr,int len,char* buffer);
void SysHalt();
int SysAdd(int op1, int op2);

void _CaseHalt(){
  DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
  SysHalt();
}

void _CaseAdd(){
  int result;
  DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << 
            " + " << kernel->machine->ReadRegister(5) << "\n");

  /* Process SysAdd Systemcall*/
  result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
          /* int op2 */(int)kernel->machine->ReadRegister(5));

  DEBUG(dbgSys, "Add returning with " << result << "\n");
  /* Prepare Result */
  kernel->machine->WriteRegister(2, (int)result);

  _IncreaseProgramCounter();
}

void _CaseReadNum(){
  char c;
  int max_int = 0;
  int negative = 1;
  long result = 0;

  // calculate MAX_INT
  if (sizeof(int) == 2) max_int = 32767;
  if (sizeof(int) == 4) max_int = 2147483647;

  while (true){
    c = kernel->synchConsoleIn->GetChar();
    if (result > 0 && (c == '\0' || c == '\n' || c == ' ')) break; // handling junk from previous input
    if (c == '\n' || c == ' ') continue;
    if (result == 0 && c == '-'){
      negative = -1;
      continue;
    }
    if (c < '0' || c > '9') {
      result = 0;
      break;
    }
    result = result*10 + ((int)c - 48);
    if (result > max_int){
      result = 0;
      break;
    }

  }

  result *= negative;

  kernel->machine->WriteRegister(2, (int)result);
  _IncreaseProgramCounter();
}

void _CasePrintNum(){
  int x = (int)kernel->machine->ReadRegister(4);
  int i = 0;
  int j;

  if (x < 0){
    kernel->synchConsoleOut->PutChar('-');

    x = -x;
  }

  if (x == 0) {
    kernel->synchConsoleOut->PutChar('0');
    _IncreaseProgramCounter();
    return;
  }

  memset(_num_buf, '\0', MAX_INT_LENGTH + 1);

  i = 0;
  while (x){
    _num_buf[i] = (char) (x % 10 + 48);
    ++i;
    x = x / 10;
  }

  // reverselt print the array
  for (j = i-1; j >= 0; --j) kernel->synchConsoleOut->PutChar(_num_buf[j]);
  _IncreaseProgramCounter();

}

void _CaseReadChar(){
  char c = kernel->synchConsoleIn->GetChar();
  kernel->machine->WriteRegister(2,  c);
  _IncreaseProgramCounter();
}

void SysPrintChar(char c){
  kernel->synchConsoleOut->PutChar(c);
}

void _CasePrintChar(){
  char c = kernel->machine->ReadRegister(4);
  SysPrintChar(c);
  _IncreaseProgramCounter();
}

void _CaseRandomNum(){
  int x = random();
  kernel->machine->WriteRegister(2, x);
  _IncreaseProgramCounter();
}


void _CaseReadString(){
  char c;
  int ptr = (int)kernel->machine->ReadRegister(4);
  int length = (int)kernel->machine->ReadRegister(5);
  int i = 0;

  if (length > MAX_STRING_LENGTH){
    DEBUG(dbgSys, "String length exceeds 255 characters");
    SysHalt();
		return;
  }

  memset(_str_buf, '\0', length+1);

  
  for (i = 0; i < length; ++i) {
    c = kernel->synchConsoleIn->GetChar();
    if (c == '\n' && i == 0) c = kernel->synchConsoleIn->GetChar();
    _str_buf[i] = c;

    if(i > 0 && c == '\0'){
      --i;
      break;
    }
  }
  _str_buf[i+1] = '\0';

  System2User(ptr, length+1, _str_buf);
  _IncreaseProgramCounter();
}

void SysPrintString(char *buf){
  int n;
  int i;
  if (buf == 0 || strlen(buf) == 0){
    return;
  }
  n = strlen(buf);
  for (i = 0; i < n; ++i) if (buf[i] == '\0'){
    return;
  } else kernel->synchConsoleOut->PutChar(buf[i]);
}

void _CasePrintString(){
  char *buf;
  int ptr;
  ptr = kernel->machine->ReadRegister(4);
  if (ptr == 0){
    _IncreaseProgramCounter();
    return;
  }

  buf = User2System(ptr, 255);
  SysPrintString(buf);
  _IncreaseProgramCounter();
}


void _IncreaseProgramCounter(){
  /* Modify return point */
  //{
  /* set previous programm counter (debugging only)*/
  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

  /* set next programm counter for brach execution */
  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
  //}
}

/* 
Input: - User space address (int) 
 - Limit of buffer (int) 
Output:- Buffer (char*) 
Purpose: Copy buffer from User memory space to System memory space 
*/ 
char* User2System(int virtAddr,int limit) 
{ 
  int i;// index 
  int oneChar; 
  char* kernelBuf = NULL; 

  kernelBuf = new char[limit +1];//need for terminal string 
  if (kernelBuf == NULL) return kernelBuf; 
  memset(kernelBuf,0,limit+1); 

  //printf("\n Filename u2s:"); 
  for (i = 0 ; i < limit ;i++) 
  { 
    kernel->machine->ReadMem(virtAddr+i,1,&oneChar); 
    kernelBuf[i] = (char)oneChar; 
    //printf("%c",kernelBuf[i]); 
    if (oneChar == 0) 
    break; 
  } 

  return kernelBuf; 
} 


/* 
Input: - User space address (int) 
- Limit of buffer (int) 
- Buffer (char[]) 
Output:- Number of bytes copied (int) 
Purpose: Copy buffer from System memory space to User memory space 
*/ 
int System2User(int virtAddr,int len,char* buffer) 
{ 
  int i;
  int oneChar;

  if (len < 0) return -1; 
  if (len == 0)return len; 

  i = 0; 
  oneChar = 0 ; 

  do{ 
    oneChar= (int) buffer[i]; 
    kernel->machine->WriteMem(virtAddr+i,1,oneChar); 
    i ++; 
  } while(i < len && oneChar != 0); 

  return i; 
} 

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}




#endif /* ! __USERPROG_KSYSCALL_H__ */
