// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

void _SyscallHandler(int&);


void
ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);
	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
	case SyscallException:
		_SyscallHandler(type);
		break;

	case NoException:
		break;

	case PageFaultException:
		SysHalt();
		break;
	case ReadOnlyException:
		SysHalt();
		break;
	case BusErrorException:
		SysHalt();
		break;
	case AddressErrorException:
		SysHalt();
		break;
	case OverflowException:
		SysHalt();
		break;
	case IllegalInstrException:
		SysHalt();
		break;
	case NumExceptionTypes:
		SysHalt();
		break;


	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		ASSERTNOTREACHED();
		break;
    }

    
}


void _SyscallHandler(int &type){
	

	switch(type) {
	case SC_Halt:
		_CaseHalt();
		return;
		ASSERTNOTREACHED();
		break;

	case SC_Add:
		_CaseAdd();
		return;
		ASSERTNOTREACHED();
		break;
	
	case SC_ReadNum:
		_CaseReadNum();
		return;
		ASSERTNOTREACHED();
		break;
	
	case SC_PrintNum:
		_CasePrintNum();
		return;
		break;

	case SC_ReadChar:
		_CaseReadChar();
		return;
		ASSERTNOTREACHED();
		break;

	case SC_PrintChar:
		_CasePrintChar();
		return;
		ASSERTNOTREACHED();
		break;

	case SC_RandomNum:
		_CaseRandomNum();
		return;
		ASSERTNOTREACHED();
		break;

	case SC_ReadString:
		_CaseReadString();
		return;
		ASSERTNOTREACHED();
		break;

	case SC_PrintString:
		_CasePrintString();
		return;
		ASSERTNOTREACHED();
		break;

	default:
		cerr << "Unexpected system call " << type << "\n";
		break;
    }
}