#include "syscall.h"

char buf[255];
char buf2[255];
char buf3[255];

// Open file by name
int FileOpenName(char* name, int *fileid) {
    *fileid = 0;

    PrintString("Open file id: ");
    *fileid = Open(name);
    PrintNum(*fileid);
    PrintString("\n");
    return *fileid;
}

// Close file after opened
void FileAutoClose(int autoclose, int *fileid) {
    int checkclose = 0;

    if (autoclose) {
        PrintString("Closing file: ");
        checkclose = Close(*fileid);
        PrintNum(checkclose);
        PrintString("\n");
    }
}

// Read file after opened
void FileRead(char* read, int *readlen, int fileid) {
    PrintString("Reading file: ");
    *readlen = Read(read, 255, fileid);
    PrintNum(*readlen);
    PrintString("\n");
}

// write to file after opened
int FileWrite(char* write, int writelen, int fileid) {
    PrintString("Writing file: ");
    writelen = Write(write, writelen, fileid);
    PrintNum(writelen);
    PrintString("\n");

    return writelen;
}

// Create new file by name, close any opened same-name file if exists
int ProgCreateEmptyFile(char* name) {
    int check = 0;
    int fileid = 0;

    PrintString("Creating Empty File: ");
    PrintString(name);
    PrintString("\n");

    PrintString("Creating file: ");
    check = Create(name);
    PrintNum(check);
    PrintString("\n\n");

    return check+1;
}

// Cat file by name
void ProgCat(char* read, int *readlen, char* name, int namelen, int autoclose) {
    int fileid = 0;

    PrintString("Catting File: ");
    PrintString(name);
    PrintString("\n");

    if (FileOpenName(name, &fileid)) {

        FileRead(read, readlen, fileid);

        if (*readlen != 0) {
            PrintString("File content: ");
            PrintString(read);
            PrintString("\n");
        }

        FileAutoClose(autoclose, &fileid);
    }

    PrintString("\n");
}

void ProgCreate(char* write, int writelen, char* name, int namelen, int autoclose) {
    int fileid = 0;

    PrintString("Creating File: ");
    PrintString(name);
    PrintString("\n");

    if (ProgCreateEmptyFile(name)) {

        if (FileOpenName(name, &fileid)) {

            // write first content to file;
            FileWrite(write, writelen, fileid);

            FileAutoClose(autoclose, &fileid);
        }
    }

    PrintString("\n");
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
    PrintString("\n");
    
    PrintString("File name b: ");
    name2len = ReadString(name2, 255);
    PrintString("\n");

    ProgCat(read, &readlen, name1, name1len, 1);
    ProgCreate(read, readlen, name2, name2len, 1);

    Halt();
    return 0;
}