// addrspace.h 
//	Data structures to keep track of executing user programs 
//	(address spaces).
//
//	For now, we don't keep any information about address spaces.
//	The user level CPU state is saved and restored in the thread
//	executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

class MemoryManager;

#include "copyright.h"
#include "filesys.h"
#include "memorymanager.h"
#include "pcb.h"

#define UserStackSize		1024 	// increase this as necessary!
class pcb; 

class AddrSpace {
  public:
    AddrSpace();
    AddrSpace(OpenFile *executable);	// Create an address space,
					// initializing it with the program
					// stored in the file "executable"
    ~AddrSpace();			// De-allocate an address space
    AddrSpace( const AddrSpace &input);
    bool Translate(int virtAddr);
    int ReadFile(int virtAddr, OpenFile* file, int size, int fileAddr);
    void InitRegisters();		// Initialize user-level CPU registers,
					// before jumping to user code
 
    void SaveState();			// Save/restore address space-specific
    void RestoreState();		// info on a context switch 
    bool  check();
    void getString(char * str, int virtAd);
    void execThread(OpenFile *executable);
    int getPhyAddr(int virtAddr);
    
    void setPCB(pcb *input);
      pcb* getPCB();
      int getPID();
      int getNumPages();
      TranslationEntry * getPageTable();
       void SaveReg();
        void RestoreReg();
    


  private:
    TranslationEntry *pageTable;	// Assume linear page table translation
					// for now!
    unsigned int numPages;		// Number of pages in the virtual 
					// address space
    MemoryManager* manager;
    bool enoughPgs;
    int pageIndex;
      pcb* thisPCB;
      int regArray[NumTotalRegs];
};

#endif // ADDRSPACE_H
