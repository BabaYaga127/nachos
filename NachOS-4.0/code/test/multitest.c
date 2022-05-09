#include "syscall.h"

int main(){
    int result;
    char c;
    char arr[256];

    result = ReadNum();
    PrintNum(result);
    PrintChar('\n');

    result = RandomNum();
    PrintNum(result);
    PrintChar('\n');
    
    c = ReadChar();
    PrintChar(c);
    PrintChar('\n');

    ReadString(arr, 5);
    PrintString(arr);
    PrintChar('\n');

    PrintString("duma duoc ak\n");

    Halt();
    return 0;
}