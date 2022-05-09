#include "syscall.h"

int main(){
    int i;
    char c;
    PrintString("ASCII TABLE FROM 32 to 126:\n\n");

    for (i = 0; i < 3; ++i) PrintString("\tDEC\tChr ");
    PrintChar('\n');
    for (c = 32; c <= 126; ++c){
        PrintChar('\t');
        PrintNum((int) c);
        PrintChar('\t');
        PrintChar(c);

        if (((int)c - 31) % 3 == 0) PrintChar('\n');
    }

    PrintString("\n\n");

    Halt();
    return 0;
}