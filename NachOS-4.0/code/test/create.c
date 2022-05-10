#include "syscall.h"

char buf[255];
char buf2[255];

void ProgCreate(char* write, int writelen, char* name, int namelen, int autoclose) {
    int check;
    int fileid = 0;
    int checksize = 0;
    int checkclose = 0;

    check = Create(name);

    PrintString("Create file status: ");
    PrintNum(check);
    PrintString("\n\n");

    if (check != -1) {
        PrintString("Opening file: ");
        fileid = Open(name);
        PrintNum(fileid);
        PrintString("\n\n");

        if (fileid != 0) {
            PrintString("Writing file: ");
            checksize = Write(write, writelen, fileid);
            PrintNum(checksize);
            PrintString("\n\n");

            if (autoclose) {
                PrintString("Closing file: ");
                checkclose = Close(fileid);
                PrintNum(checkclose);
                PrintString("\n\n");
            }
        }
    }
}

int main(){
    char* name = buf;
    int namelen = 0;
    char* write = buf2;
    int writelen = 0;

    PrintString("Creating file...\n");

    PrintString("File name: \n");
    namelen = ReadString(name, 255);
    PrintChar('\n');

    PrintString("File content: \n");
    writelen = ReadString(write, 255);
    PrintChar('\n');

    ProgCreate(write, writelen, name, namelen, 1);

    Halt();
    return 0;
}