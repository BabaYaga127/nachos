#include "syscall.h"

char buf[255];
char buf2[255];
char buf3[255];
char buf4[255];

void ProgCat(char* read, int *readlen, char* name, int namelen, int autoclose) {
    int fileid = 0;
    int checkclose = 0;

    PrintString("Cat file id: ");
    fileid = Open(name);
    PrintNum(fileid);
    PrintString("\n");

    if (fileid != 0) {
        PrintString("Reading file: ");
        *readlen = Read(read, 255, fileid);
        PrintNum(*readlen);
        PrintString("\n");

        if (*readlen != 0) {
            PrintString("File content: \n");
            PrintString(read);
            PrintString("\n");
        }


        if (autoclose) {
            PrintString("Closing file: ");
            checkclose = Close(fileid);
            PrintNum(checkclose);
            PrintString("\n\n");
        }
        
    }
}

int ProgCreate(char* write, int writelen, char* name, int namelen, int autoclose) {
    int check;
    int fileid = 0;
    int checksize = 0;
    int checkclose = 0;

    check = Create(name);

    PrintString("Create file status: ");
    PrintNum(check);
    PrintString("\n");

    if (check != -1) {
        PrintString("Opening file: ");
        fileid = Open(name);
        PrintNum(fileid);
        PrintString("\n");

        if (fileid != 0) {
            PrintString("Writing file: ");
            checksize = Write(write, writelen, fileid);
            PrintNum(checksize);
            PrintString("\n");

            if (autoclose) {
                PrintString("Closing file: ");
                checkclose = Close(fileid);
                PrintNum(checkclose);
                PrintString("\n\n");
            }

            return fileid;
        }
    }

    return 0;
}

int ProgWriteAt(int pos, char* write, int writelen, char* name, int namelen, int autoclose) {
    int fileid = 0;
    int checkclose = 0;
    int seek = 0;

    PrintString("Write file id: ");
    fileid = Open(name);
    PrintNum(fileid);
    PrintString("\n");

    if (fileid != 0) {
        PrintString("Seeking ");
        PrintNum(pos);
        PrintString(" : ");
        seek = Seek(pos, fileid);
        PrintNum(seek);
        PrintString("\n");

        PrintString("Writing file: ");
        writelen = Write(write, writelen, fileid);
        PrintNum(writelen);
        PrintString("\n");

        if (autoclose) {
            PrintString("Closing file: ");
            checkclose = Close(fileid);
            PrintNum(checkclose);
            PrintString("\n\n");
        }
        
    }
}

int main(){
    char *name1 = buf;
    int name1len = 0;
    char *read1 = buf2;
    int read1len = 0;
    char *name2 = buf3;
    int name2len = 0;
    char* read2 = buf4;
    int read2len = 0;
    int i;

    PrintString("Concatenate: \n");

    PrintString("file name a: ");
    name1len = ReadString(name1, 255);
    PrintString("\n");

    PrintString("file name b: ");
    name2len = ReadString(name2, 255);
    PrintString("\n");

    ProgCat(read1, &read1len, name1, name1len, 1);
    ProgCat(read2, &read2len, name2, name2len, 1);

    ProgCreate(read1, read1len, "concat.txt", 10, 0);
    ProgWriteAt(read1len, read2, read2len, "concat.txt", 10, 1);

    for (i = 0; i < read1len; i++) read1[i] = '\0';
    read1len = 0;
    ProgCat(read1, &read1len, "concat.txt", 10, 1);

    Halt();
    return 0;
}