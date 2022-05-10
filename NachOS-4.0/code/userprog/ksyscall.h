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
#include "hash.h"
#include "directory.h"

#include <stdio.h>

#define MAX_INT_LENGTH 12
char _num_buf[MAX_INT_LENGTH+1];

#define MAX_STRING_LENGTH 255
char _str_buf[MAX_STRING_LENGTH+1];

// project 01
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

// project 02

class MyOpenFile{
  public:
  OpenFile* file;
  char* name;
};

void _CaseCreate();
void SysCreate(char *name);

void _CaseRemove();
void SysRemove(char* name);

void _CaseOpen();
void SysOpen(char* name);
char* SysGetKeyFromMyOpenFile(MyOpenFile *file);
unsigned SysGetHashFromKey(char *key);

void _CaseCLose();

void _CaseRead();
void SysRead(char* buf, int size, MyOpenFile* file);

void _CaseWrite();
void SysWrite(char* buf, int size, MyOpenFile* file);

void _CaseSeek();
void SysSeek(int pos, char* name);


char* SysGetKeyFromMyOpenFile(MyOpenFile *file) {
  if (file == 0) {
    return 0;
  }

  return file->name;
}

unsigned int SysGetHashFromKey(char *key) {
  if (key == 0) {
    return 0;
  }

  unsigned int hash = 0;
  for (unsigned int i = 0; i < strlen(key); ++i) {
    char c = *(key+i);
    hash = ((hash << 5) - hash) + c; 
    hash |= 0;
  }

  return hash;
}

HashTable<char*, MyOpenFile*> _unclose_file(&SysGetKeyFromMyOpenFile, &SysGetHashFromKey);

void testhash(char* name) {
  MyOpenFile *file = new MyOpenFile();
  MyOpenFile *tmp = 0;
  
  file->file = kernel->fileSystem->Open(name);
  file->name = name;

  SysPrintString("Push: \n");
  std::cerr << (int)(file) << '\n';
  _unclose_file.Insert(file);

  file->file->Write("duma", 4);

  SysPrintString("Find: \n");
  std::cerr << _unclose_file.Find(file->name, &tmp) << '\n';
  std::cerr << (int)(tmp) << '\n';

  tmp->file->Write("gcc", 3);

  if (tmp != 0) {
    SysPrintString("Found: \n");
    SysPrintString(tmp->name);
    SysPrintString("\n");
  } 

  SysPrintString("Content: \n");

  memset(_str_buf, '\0', 10);
  tmp->file->ReadAt(_str_buf, 10, 0);

  SysPrintString(_str_buf);

  SysPrintString("Remove: \n");
  _unclose_file.Remove(file->name);

  delete file;
}

void SysWrite(char* buf, int size, MyOpenFile* file) {
  int check = file->file->Write(buf, size);
  kernel->machine->WriteRegister(2, check);
}

void _CaseWrite() {
  char *buf = 0;
  MyOpenFile* file = 0;
  int size;

  buf = (char*)kernel->machine->ReadRegister(4);
  size = kernel->machine->ReadRegister(5);
  file = (MyOpenFile*)kernel->machine->ReadRegister(6);
  if (buf == 0 || size == 0 || file == 0) {
    SysPrintString("cannot write to file\n\n");
    kernel->machine->WriteRegister(2, 0);
    _IncreaseProgramCounter();
    return;
  }
  buf = User2System((int)buf, size);
  SysWrite(buf, size, file);
  _IncreaseProgramCounter();
}

void SysRead(char* buf, int size, MyOpenFile* file) {
  memset(_str_buf, '\0', size+1);
  int check = file->file->Read(_str_buf, size);
  System2User((int)buf, size, _str_buf);

  kernel->machine->WriteRegister(2, check);
}

void _CaseRead() {
  char *buf = 0;
  MyOpenFile* file = 0;
  int size;

  buf = (char*)kernel->machine->ReadRegister(4);
  size = kernel->machine->ReadRegister(5);
  file = (MyOpenFile*)kernel->machine->ReadRegister(6);
  if (buf == 0 || size == 0 || file == 0) {
    kernel->machine->WriteRegister(2, 0);
    _IncreaseProgramCounter();
    return;
  }

  SysRead(buf, size, file);
  _IncreaseProgramCounter();
}

void SysSeek(int pos, MyOpenFile* file) {
  SysPrintString("seeking file: \n");

  if (pos == -1) 
    pos = file->file->Length()-1;
  pos = max(min(pos, file->file->Length()-1), 0);

  memset(_str_buf, '\0', 5);
  int check = file->file->ReadAt(_str_buf, 1, pos);
  if (check == 0) {
    SysPrintString("error pos \n");
    kernel->machine->WriteRegister(2, -1);
    return;
  }

  SysPrintString("seek success \n");
  file->file->Seek(pos);
  kernel->machine->WriteRegister(2, pos);
}

void _CaseSeek(){
  char *name;
  int pos;
  MyOpenFile *file = 0;
  pos = kernel->machine->ReadRegister(4);
  file = (MyOpenFile*)kernel->machine->ReadRegister(5);
  if (file == 0) {
    SysPrintString("cannot seek file\n\n");
    kernel->machine->WriteRegister(2, -1);
    _IncreaseProgramCounter();
    return;
  }

  SysSeek(pos, file);
  _IncreaseProgramCounter();
}

void SysRemove(char* name) {
  MyOpenFile* file = 0;
  if (_unclose_file.Find(name, &file)) {
    _unclose_file.Remove(name);
    delete file;
  }

  int check = (int)kernel->fileSystem->Remove(name);
  kernel->machine->WriteRegister(2, check-1);
}

void _CaseRemove() {
  char *name;
  int ptr;
  ptr = kernel->machine->ReadRegister(4);
  if (ptr == 0) {
    SysPrintString("cannot remove file\n");
    _IncreaseProgramCounter();
    return;
  }

  name = User2System(ptr, 255);
  SysRemove(name);
  _IncreaseProgramCounter();
}

void _CaseClose() {
  MyOpenFile *file = 0;
  int ptr = kernel->machine->ReadRegister(4);
  file = (MyOpenFile*)ptr;
  if (file == 0) {
    kernel->machine->WriteRegister(2, -1);
    _IncreaseProgramCounter();
    return;
  }

  _unclose_file.Remove(file->name);
  delete file;
  kernel->machine->WriteRegister(2, 0);
  _IncreaseProgramCounter();
}

void SysOpen(char* name) {
  MyOpenFile* file = 0;

  if (!_unclose_file.Find(name, &file)) {
    SysPrintString("Open closed file: ");
    file = new MyOpenFile();
    file->file = kernel->fileSystem->Open(name);
    file->name = name;
    _unclose_file.Insert(file);
  } else {
    SysPrintString("Open opened file: ");
  }

  SysPrintString(name);
  SysPrintString("\n");

  if (file->file == 0) {
    kernel->machine->WriteRegister(2, 0);
    return;
  }

  kernel->machine->WriteRegister(2, (int)file);
}

void _CaseOpen() {
  char *name;
  int ptr;
  ptr = kernel->machine->ReadRegister(4);
  if (ptr == 0) {
    SysPrintString("cannot open file\n\n");
    _IncreaseProgramCounter();
    return;
  }

  name = User2System(ptr, 255);
  SysOpen(name);
  _IncreaseProgramCounter();
}

void SysCreate(char *name) {
  bool check;
#ifdef FILESYS_STUB
  check = kernel->fileSystem->Create(name);
#else 
  check = kernel->fileSystem->Create(name, 0);
#endif
  
  
  if (check) 
    kernel->machine->WriteRegister(2, 0);
  else 
    kernel->machine->WriteRegister(2, -1);

  //testhash(name);
}

void _CaseCreate() {
  char *name;
  int ptr;
  ptr = kernel->machine->ReadRegister(4);
  if (ptr == 0) {
    SysPrintString("cannot create file\n\n");
    kernel->machine->WriteRegister(2, -1);
    _IncreaseProgramCounter();
    return;
  }

  name = User2System(ptr, 255);
  SysCreate(name);
  _IncreaseProgramCounter();
}

// =========================================================================

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

    if(i > 0 && (c == '\0' || c == '\n')){
      --i;
      break;
    }
  }
  _str_buf[i+1] = '\0';
  kernel->machine->WriteRegister(2, i+1);

  System2User(ptr, i+1, _str_buf);
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
