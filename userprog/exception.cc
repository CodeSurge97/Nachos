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
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "machine.h"
#include "pcb.h"
#include "addrspace.h"
#include "processmanager.h"

class pcb;
class ProcessManager;
class AddrSpace;

void helpFork(int i);
int syscallFork();
void syscallExit();
void syscallYield();
int syscallExec();
int syscallJoin();

int counter;

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
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
int type = machine->ReadRegister(2);

if ((which == SyscallException))
{
    switch(type)
    {
        case  SC_Halt:
            {
                DEBUG('a', "Shutdown, initiated by user program.\n");
                interrupt->Halt(); 
                break;
            }
            
        case SC_Fork:
            {
                DEBUG('a', "Fork, initiated by user program.\n");
                syscallFork();
                break;
            }
        case SC_Yield:
            {
                DEBUG('a', "Yield, initiated by user program.\n");
                syscallYield();
                break;
            }
            
        case SC_Exit:
            {
                DEBUG('a', "Exit, initiated by user program.\n");
                syscallExit();
                break;
            }
            
        case SC_Exec:
            {
                DEBUG('a', "Exec, initiated by user program.\n");
                syscallExec();
                break;
            }
            
        /* case SC_Kill:
            {
                printf("System Call: [%d] invoked Fork.\n", currentThread->space->getPID()); 
                break;
            }
        */  
        case SC_Join:
            {
                DEBUG('a', "Join, initiated by user program.\n");
                syscallJoin();
                break;
            }
        case SC_Create:
            int virtualAd = machine -> ReadRegister(4);
            char *FileName = new char[256];
            currentThread -> space -> getString(fileName, virtualAd);
            fileSystem -> Create(fileName, 0);
            break;
        case SC_Open:
            int virtualAd = machine -> ReadRegister(4);
            char *FileName = new char[256];
            currentThread -> space -> getString(fileName, virtualAd);

            //Add new process 'open File' to your list of your process' open files
            /*Code here*/

            //check to see if the system has this file open yet
            if(/*fileopen?*/){
                //system file table increments the counter for this file
            }
            else{
                //add a new system open file
                fileSystem -> Open(filenName);
            }
        case SC_Close:
            int virtualAd = machine -> ReadRegister(4);

            OpenFile *openedFile = (OpenFile*)virtualAd;
            fileSystem -> Remove(/*FileName*/);
            
            //Using the file id passed in(how do we get the id?), remove open file from process list of
            //open files
            /*File ID*/

            //it in turn will let the system file table know to remove the file if necessary
            if(count > 0){
                count--;
            }
            else{
                //remove file from list and close it
                fileSystem ->
            }
        
    }

    
} 
else 
{
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
}

}
/*           
Yield
//Steps:
//  1. Save currentThread state
//  2. Call Thread::Yield() 
//==================================================================================

    currentThread->Yield();
    break;
*/      
void
syscallYield()
{
printf("System Call: [%d] invoked Yield.\n", currentThread->space->getPID()); 
currentThread->Yield();

//update counter
        counter = machine->ReadRegister(PCReg);
        counter += 4;
        machine->WriteRegister(PrevPCReg, counter-4);
        machine->WriteRegister(PCReg, counter);
        machine->WriteRegister(NextPCReg, counter+4);

}

/* The Exit(int) call takes a single argument, which is an integer status value as 
in Unix. The currently executing process is terminated. For now, you can just 
ignore the status value. Later you will figure out how to get this value to an 
interested process.
1. Get exit code from r4;
if current process has children, set their parent pointers to null;
if current process has a parent, remove itself from the children list of its parent process and set child exit value to parent.
2. Remove current process from the pcb manager and pid manager.
3. Deallocate the process memory and remove from the page table;
current thread finish.
==================================================================================
*/
void 
syscallExit()
{
printf("System Call: [%d] invoked Exit.\n", currentThread->space->getPID()); 

int i, exitStatus;
int pid =currentThread->space->getPID();

exitStatus = machine->ReadRegister(4);

//if current process has children, set their parent pointers to null
if(currentThread->space->getPCB()->numberChildren() > 0)
{
	currentThread->space->getPCB()->setParentsNull();
    exitStatus =1;
}
//if one thread halt machine
if(currentThread->space->getPCB()->numberChildren() <=1)
{
    //  printf("only one or less children, machine will halt!\n");
    interrupt->Halt();
    
}
//if this process has a parent, remove itself from the children list of the parent process and set exit value to parent 
if(currentThread->space->getPCB()->getParent() != NULL)
{
	currentThread->space->getPCB()->getParent()->removeChild(pid);
	currentThread->space->getPCB()->getParent()->setChildExitValue(exitStatus);
}

//Remove current process from the pcb manager and pid manager.
pcbMan->removePCB(currentThread->space->getPID());
pid_manager->removePid(currentThread->space->getPID());

//Deallocate the process memory and remove from the page table; current thread finish.
AddrSpace *addSpace = currentThread->space;
TranslationEntry *page = addSpace->getPageTable();
for(i=0; i < addSpace->getNumPages(); i++)
{
	    memMan->clearPage(page[i].physicalPage);
}

printf("Process [%d] exits with [%d]\n", pid, exitStatus);

//if this is the last process, then just exit
delete addSpace;
currentThread->Finish();

}
/* Join
STEPS
1. Read process id from register r4.
2. Make sure the requested process id is the child process of the current process.
3. Keep on checking if the requested process is finished. if not, yield the current process.
4. If the requested process finished, write the requested process exit id to register r2 to return it.
*/
int 
syscallJoin()
{

printf("System Call: [%d] invoked Join.\n", currentThread->space->getPID()); 

//Read process id from register r4.
int pid = machine->ReadRegister(4);

// Make sure the requested process id is the child process of the current process.
	if(currentThread->space->getPCB()->getParent()->getID() == pid)
	{
	        machine->WriteRegister(2, -1);
		return -1;
	}

if(currentThread->space->getPCB()->checkForChild(pid))
{
//Keep on checking if the requested process is finished. if not, yield the current process.
    while(currentThread->space->getPCB()->checkForChild(pid))
	{
		    currentThread->Yield();
	}
//If the requested process finished, write the requested process exit id to register r2 to return it.
    machine->WriteRegister(2, currentThread->space->getPCB()->getChildExitValue());
	    return (currentThread->space->getPCB()->getChildExitValue());
}
else
{
    machine->WriteRegister(2, -1);
}

return -1; 
}
/* The Exec(filename) system call spawns a new user-level thread (process), but 
creates a new address space and begins executing a new program given by the 
object code in the Nachos file whose name is supplied as an argument to the 
call. It should return to the parent a SpaceId which can be used to uniquely 
identify the newly created process.
*/

//Steps:
//  1. Need method for transferring data(name of executable as arg for syscall)
//  2. Verify file exists
//  3. Consult the  exec. file to determine amount of physical memory is required 
//     for new program (physical mem should be allocated and initialized w/data 
//     from exec. file)
//  4. Page table thread adjusted for new program, MIPS registers reinitialized
//     for starting at beginning of new program, control should return to user mode
//     (See progtest.cc)
// NOTE: Using "machine->Run" to execute user program terminates the current thread.
//       Find a way to return space ID for Exec to work 
//==================================================================================
int 
syscallExec()
{

printf("System Call: [%d] invoked Exec.\n", currentThread->space->getPID()); 

//Read register r4 to get the executable path.
int virtualAd = machine->ReadRegister(4);
char * fileName = new char[100];

//Replace the process memory with the content of the executable.
currentThread->space->getString(fileName, virtualAd);


OpenFile *executable = fileSystem->Open(fileName); 
if(executable  == NULL) 
{
	//printf("Unable to open file %s\n", fileName);	
    return -1;
}

AddrSpace *addSpace = new AddrSpace(executable);
currentThread->space = addSpace;


delete executable;

addSpace->InitRegisters();		// set the initial register values
addSpace->RestoreState();		// load page table register

	if(currentThread->space->check() == false)
	{ 
	    counter = machine->ReadRegister(PCReg);
    counter += 4;
    machine->WriteRegister(PrevPCReg, counter-4);
    machine->WriteRegister(PCReg, counter);
    machine->WriteRegister(NextPCReg, counter+4);
	    return -1; 
	}

	printf("Exec Program: [%d] loading [%s]\n", currentThread->space->getPID(), fileName);

//Write 1 to register r2 indicating exec() invoked successful;
	machine->WriteRegister(2,1);
}

void 
dummyFunx(int i)
{
currentThread->space->RestoreReg();
currentThread->space->RestoreState();
machine->Run();
}
/* Starts a new process which runs as a user function 
specified by the argument of the call with the function
being compiled as part of the user program via the parent

User program expects a new thread to be used and the thread
will run the function with same exact address space
*/

//Steps:
//   -Find entry point of function that is passed in the parameter
//       -Insert code into exception handler
//          1. Set up an address space which is a copy of the current thread's space
//          2. Load address that is in register 4 into the program counter
//          3. Use Thread::Fork(), initialize MIPS registers for the new process
//          4. Have both processes return to user mode
//          5. Parent return to user mode by returning from the exception handler
//             (child process should continue to run from the entry point of the function)
//==================================================================================

int 
syscallFork()
{

DEBUG('a', "Fork, initiated by user program.\n");
Lock *memLock;
memLock = new Lock("memLock");

//save old process registers
currentThread->space->SaveState();

//create a new thread   
Thread *child = new Thread("ForksThread");

//create a new addr.sp 
    AddrSpace *newSpace = new AddrSpace();

printf("System Call: [%d] invoked Fork.\n", currentThread->space->getPID()); 

//copy old addr.sp to new addr.sp associated with child   
    child->space = newSpace;
        
//associate pcb with new addr.sp and new thread
    newSpace->getPCB()->setThread(child);
    
(currentThread->space->getPCB())->setParent((child->space->getPCB()));
(child->space->getPCB())->addChild((currentThread->space->getPCB()));

//check if enough memory

if(!newSpace->check())
{
    // printf("Not enough mem: tempAd check returned false");
    machine->WriteRegister(2, -1);
	    return -1;
}

//copy old reg values to new reg values

for(int i = 0; i < NumTotalRegs; i++)
{
    child->SetUserRegister(i, currentThread->GetUserRegister(i));
}
//set pc reg to value in r4
child->SetUserRegister(PCReg, machine->ReadRegister(4)); 

//save new reg values to new state   
newSpace->SaveState();
//new thread runs dummy funx that will copy back machine registers
child->Fork(dummyFunx,1); 

printf("Process [%d] Fork: start at address [0x%x] with [%d] pages memory\n", currentThread->space->getPID(), machine->ReadRegister(4), currentThread->space->getNumPages());


//restore reg
currentThread->space->RestoreReg();

machine->WriteRegister(2, newSpace->getPID());
memLock->Release();
    
//update counter
counter = machine->ReadRegister(PCReg);
// counter += 4;
machine->WriteRegister(PrevPCReg,counter);
counter = counter + 4;
machine->WriteRegister(PCReg,counter);
counter = counter + 4;
machine->WriteRegister(NextPCReg,counter);

return currentThread->space->getPID();

}

