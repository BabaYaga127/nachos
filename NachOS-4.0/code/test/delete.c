#include "syscall.h"

char buf[255];

int ProgDelete(char* name, int namelen) {
    int check = 0;

    PrintString("Deleting file: ");
    check = Remove(name);
    PrintNum(check);
    PrintString("\n\n");

    return check;
}

int main(){
    char* name = buf;
    int namelen = 0;


    PrintString("Deleting file...\n");

    PrintString("File name: ");
    namelen = ReadString(name, 255);
    PrintString("\n\n");
    
    ProgDelete(name, namelen);

    Halt();
    return 0;
}