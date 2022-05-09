#include "syscall.h"

int main(){
    PrintString("TEAM MEMBER\n");

    PrintString("1. Bui Quang Minh - 19125010\n");
    PrintString("2. Phan Dam Tung Lam - 19125102\n\n");

    PrintString("Available Program: ascii \n\t\tprint out all characters \n\t\tfrom 32-' ' to 126-'~')\n");
    PrintString("-> How to run: \n\t../build.linux/nachos -rs 1023 -x ascii\n\n");
    
    PrintString("Available Program: sort \n\t\tread n-length of integer array, \n\t\tthen read n integer, \n\t\tprint out sorted array)\n");
    PrintString("-> How to run: \n\t../build.linux/nachos -rs 1023 -x sort\n\t[Input n]\n\t[Input a[1]] [Input a[2]] ... [Input a[n]]\n\n");
    
    PrintString("\n");

    Halt();
    return 0;
}