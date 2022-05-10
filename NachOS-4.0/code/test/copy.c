#include "syscall.h"

char buf[255];
char buf2[255];
char buf3[255];
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
    char* name1 = buf;
    int name1len = 0;
    char* name2 = buf2;
    int name2len = 0;
    char* read = buf3;
    int readlen = 0;

    PrintString("Copying file...\n");

    PrintString("File name a: ");
    name1len = ReadString(name1, 255);
    PrintString("\n\n");
    
    PrintString("File name b: ");
    name2len = ReadString(name2, 255);
    PrintString("\n\n");

    ProgCat(read, &readlen, name1, name1len, 1);
    ProgCreate(read, readlen, name2, name2len, 1);

    Halt();
    return 0;
}