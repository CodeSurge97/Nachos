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
#include "console.h"
#include "addrspace.h"
#include "synch.h"
#include "syscall.h"
#include "openfile.h"
#include "thread.h"

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

    if (which == SyscallException) {
        switch(type){
            case SC_Halt:
                DEBUG('a', "Shutdown, initiated by user program.\n");
   	            interrupt->Halt();
                break;
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
//            case SC_Fork:
            //==================================================================================
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
                //Save old process registers
                /*int oldRegs [NumTotalRegs];
                for(int i = 0; i < NumTotalRegs; i++){
                    oldRegs [i] = machine -> ReadRegister(i);
                }*/
 /*               int oldReg = machine -> ReadRegister(4);
                
                OpenFile *executable = fileSystem->Open(filename);
                AddrSpace *space;
                //Error checking for file open
                if (executable == NULL) {
                    printf("Unable to open file %s\n", filename);
                    return;
                }
                //Create new AddrSpace
                space = new AddrSpace(executable);
                //Copy old AddrSpace to new one
                space = currentThread -> space;

                //Create new thread
                Thread *cThread = new Thread("Child Thread");
                //Associate new AddrSpace with new Thread
                cThread -> space = space;

                //Create new PCB
                forkPCB = new PCB();
                //Set PCB values
                forkPCB = {pid, ppid, etc.};

                //Copy old register to new register
                InitRegisters() = oldReg;
                //copy r4
                PCReg = oldReg;
                //Save new register values to new AddrSpace
                space ->InitRegisters();

                //Creates new kernel thread and sets address space to duplicate current Thread
                currentThread->Fork(setBehavior(), oldReg);
                break;
*/
/*            case SC_Yield:
            //==================================================================================
            //Steps:
            //  1. Save currentThread state
            //  2. Call Thread::Yield() 
            //==================================================================================
            
                currentThread->Yield();
                break;
*/                
//            case SC_Exec:
            //==================================================================================
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
                //Read Register to r4 to get exec path
/*                int execPath = machine -> ReadRegister(4);

                //Replace memory w content of exec

                //Init Registers
                currentThread -> space -> InitRegisters();

                if(exec == successful){
                    machine -> WriteRegister(2) = 1
                }
                else{
                    return -1;
                }
                
                break;
*/                
//            case SC_Join:
            //==================================================================================
            /* Waits and returns only after a process with the specified ID 
                (supplied as an argument to the call) has finished
            */
            //==================================================================================
/*                int result = Join(machine->ReadRegister(4));
                machine->WriteRegister(2,result);
                break;
*/                
//            case SC_Exit:
            //==================================================================================
            /* The Exit(int) call takes a single argument, which is an integer status value as 
                in Unix. The currently executing process is terminated. For now, you can just 
                ignore the status value. Later you will figure out how to get this value to an 
                interested process.
            */

            //Steps:
            //  1. Deallocating phyiscal memory that is assigned to exiting thread
            //  2. Call Thread::Finish()
            //==================================================================================
                //Get exit code from register 4
 /*               int status = machine -> ReadRegister(4);

                if (cThread != NULL){
                    *currentThread = NULL;
                }
                if()
                machine->Exit(status)
                break;
            case SC_Kill:
                break;
        }
*/	
    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
    updateCount();
}

void updateCount(){
    int counter;
    counter = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg,counter);
    counter = counter + 4;
    machine->WriteRegister(PCReg,counter);
    counter = counter + 4;
    machine->WriteRegister(NextPCReg,counter);
}

void setBehavior(){
    currentThread -> space -> InitRegisters();
    currentThread -> space -> RestoreState();
    machine -> Run;
    currentThread -> processID -> WriteRegister(2);
    updateCount();
}
