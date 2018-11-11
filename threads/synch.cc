// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// add process to list
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {
    name =debugName;
    queue = new List;
    value = 1;
    heldBy = NULL;
}
Lock::~Lock() {
    delete queue;
    heldBy = NULL;
}
void Lock::Acquire() {
    
    IntStatus oldLevel = interrupt->SetLevel(IntOff);    // disable interrupts
    
    while (value == 0) {
        queue->Append((void *)currentThread);
        currentThread->Sleep();
    }
    value--;
    heldBy = (Thread*)currentThread;
    // consume its value
    
    (void) interrupt->SetLevel(oldLevel);    // re-enable interrupts
}
void Lock::Release() {
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    if(heldBy == currentThread){
        thread = (Thread *)queue->Remove();
        if (thread != NULL)
            scheduler->ReadyToRun(thread);
        value++;
        heldBy = NULL;
    }
    
    (void) interrupt->SetLevel(oldLevel);
}

bool Lock::isHeldByCurrentThread(){
    return heldBy == currentThread;
}

Condition::Condition(char* debugName) {
    name = debugName;
    queue = new List;
}
Condition::~Condition() {
    delete queue;
    
}
//Waits for condition to become free then acquires conditionLock for current thread
void Condition::Wait(Lock* conditionLock) {
    //is condition free?
    if(conditionLock != NULL){
        //set non-interruptible
        IntStatus oldLevel = interrupt->SetLevel(IntOff);
        //release the lock
        conditionLock->Release();
        /*relinquish the CPU until signaled*/
        queue->Append((void *)currentThread);
            //set interruptible
        (void) interrupt->SetLevel(oldLevel);
        //put current thread to sleep
        currentThread->Sleep();
        /* end relinquish CPU until signaled*/
        //Acquire conditionLock for the currentThread
        conditionLock->Acquire();
        

    }
        ASSERT(FALSE);
    
}
//Wakes up ONE of the threads that is waiting on the condition
void Condition::Signal(Lock* conditionLock) {
    Thread *thread;
    //if thread is waiting
    if(conditionLock == NULL){
        //set non-interruptible
        IntStatus oldLevel = interrupt->SetLevel(IntOff);
        /*Start process for wake up*/
            //Remove it from queue
        thread = (Thread *)queue->Remove();
            //Any threads in the wait state?
        if(thread !=NULL){
            //Put in ready-to-run state
            scheduler->ReadyToRun(thread);
        }
        /*End process for wake up*/
        
        //set interruptible
        (void) interrupt->SetLevel(oldLevel);
        //wake up a thread
    }
}
//Wake up al threads waiting for the condition
void Condition::Broadcast(Lock* conditionLock) {
    Thread *thread;
    if(conditionLock == NULL){
        //set non-interruptible
        IntStatus oldLevel = interrupt->SetLevel(IntOff);
        //while there are threads in the queue wake them up
        while((thread = (Thread *)queue->Remove()) != NULL){
            scheduler->ReadyToRun(thread);
        }
        //set interruptible
        (void) interrupt->SetLevel(oldLevel);
    }
}
