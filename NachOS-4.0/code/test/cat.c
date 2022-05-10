#include "syscall.h"

char buf[255];
char buf2[255];

void ProgCat(char* read, int *readlen, char* name, int namelen, int autoclose) {
    int fileid = 0;
    int checkclose = 0;

    PrintString("Opening file: ");
    fileid = Open(name);
    PrintNum(fileid);
    PrintString("\n\n");

    if (fileid != 0) {
        PrintString("Reading file: ");
        *readlen = Read(read, 255, fileid);
        PrintNum(*readlen);
        PrintString("\n\n");

        if (*readlen != 0) {
            PrintString("File content: \n");
            PrintString(read);
            PrintString("\n\n");
        }

        if (autoclose) {
            PrintString("Closing file: ");
            checkclose = Close(fileid);
            PrintNum(checkclose);
            PrintString("\n\n");
        }
    }
}

int main(){
    char* name = buf;
    int namelen = 0;
    char* read = buf2;
    int readlen = 0;

    PrintString("Catting file...\n");

    PrintString("File name: \n");
    namelen = ReadString(name, 255);
    PrintString("\n");
    
    ProgCat(read, &readlen, name, namelen, 1);

    Halt();
    return 0;
}