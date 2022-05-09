#include "syscall.h"

#define MAX_LENGTH 100

int arr[MAX_LENGTH];

int main(){
    int n, i, j, t;

    PrintString("Input n: \n");
    n = ReadNum();
    PrintString("Input you array with n interger: \n");
    for (i = 0; i < n; ++i) {
        PrintString("Input a[");
        PrintNum(i);
        PrintString("]: ");
        arr[i] = ReadNum();
    }

    for (i = 0; i < n-1; ++i)
        for (j = i+1; j < n; ++j) if (arr[i] > arr[j]){
            t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    PrintString("\nYour sorted array is: ");
    for (i = 0; i < n; ++i) {
        PrintNum(arr[i]);
        PrintChar(' ');
    }
    
    PrintString("\n\n");
    Halt();
    return 0;
}