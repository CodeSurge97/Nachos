// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "memorymanager.h"
#ifdef HOST_SPARC
#include <strings.h>

#endif

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the 
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void 
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical 
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------
AddrSpace::AddrSpace()
{
    enoughPgs=true;
    thisPCB = new pcb(currentThread);
    thisPCB->setAddrSpace(this);
    this->setPCB(thisPCB);
    pcbMan->assignPCB(thisPCB);
    numPages=0;
    pageIndex=0;
}

AddrSpace::AddrSpace(OpenFile *executable)
{
    NoffHeader noffH;
    unsigned int i, size, counter;
    enoughPgs = true;
 
    thisPCB = new pcb(currentThread);
    thisPCB->setAddrSpace(this);
    this->setPCB(thisPCB);
    pcbMan->assignPCB(thisPCB);
    enoughPgs=true;
    
    
   
    pageIndex = 0;
    
    
  //  printf("INSIDE ADDRSPACE\n");
    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

    // how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size 
			+ UserStackSize;	// we need to increase the size
	
    // to leave room for the stack
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize; //LOGICAL ADDR. SPC. SIZE
    
   // printf(" size: %d and numPages: %d\n", size, numPages);
    printf("Loaded Program: [%d] code | [%d] data | [%d] bss\n", noffH.code.size, noffH.initData.size, noffH.uninitData.size);
    
    
    //test that there are enough pages left for this process
    if(numPages > memMan->getPage())
    {
        printf("Not Enough Memory for Process %d\n", this->getPID());
	    enoughPgs = false;
	    return;
    }
    
    
    memMan = new MemoryManager(numPages);
    
    //DEBUG STATEMENT
    // DEBUG('')
    ASSERT(numPages <= NumPhysPages);		// check we're not trying
						// to run anything too big --
						// at least until we have
						// virtual memory
    
        
    
    DEBUG('a', "Initializing address space, num pages %d, size %d\n", 
					numPages, size);
 //   printf("Initializing address space, num pages %d, size %d\n",numPages, size);

    
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++) 
    {
	pageTable[i].virtualPage = i;	
    //pages allocated by your memory manager for the physical page index.
	pageTable[i].physicalPage = memMan->getPage();
    //debug statement : print phy pg & log pg # 
	pageTable[i].valid = TRUE;
	pageTable[i].use = FALSE;
	pageTable[i].dirty = FALSE;
	pageTable[i].readOnly = FALSE;  // if the code segment was entirely on 
					// a separate page, we could set its 
					// pages to be read-only
  //  printf("PageTable Index : %d, Virtual Page : %d, Physical Page: %d \n", i, pageTable[i].virtualPage, pageTable[i].physicalPage);
  //  bzero(machine->mainMemory + (pageTable[i].physicalPage * 128), PageSize);
    }

     
// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
   bzero(machine->mainMemory, size);

  
// then, copy in the code and data segments into memory
    if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]),
			noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
			noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }

}
//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
    delete pageTable;
    delete thisPCB;
}

//----------------------------------------------------------------------
// AddrSpace::Tranlate
// 	Converts a virtual address to a physical address. First, breake the virtual address into a page table index and an offset. Then, looks up the physical page in the page table entry given by the page table index and obtains the final physical address by combining the physical page address with the page offset.
    
//  returns True OR False depending on whether or not the virtual address was valid.
//----------------------------------------------------------------------
bool
AddrSpace::Translate(int virtAddr)
{
    int physAddr, vpn, offset;
    
    vpn =  (unsigned)virtAddr / PageSize;
    offset = (unsigned) virtAddr % PageSize;
    
    // the following if statements check for invalid virtual addr 
    
    if(vpn >= (unsigned)numPages)
    {
        printf("Virtual Page : d% is too large \n", virtAddr);
        return false;
        
    }
    else if(!pageTable[vpn].valid)
    {
        printf("Virtual Page: d% it is invalid in page table \n", virtAddr);
        
        return false;
    }
    // else, valid virutal address can be translated. 
    physAddr = PageSize * pageTable[vpn].physicalPage + offset;
    
  //  printf("virtual addr : %d and physAddr: %d \n", virtAddr,physAddr);
    return true;
}
int
AddrSpace::getPhyAddr(int virtAddr)
{
  int physAddr, vpn, offset;
  vpn =  (unsigned)virtAddr / PageSize;
  offset = (unsigned) virtAddr % PageSize;
  
  if(Translate(virtAddr))
  {
    physAddr = PageSize * pageTable[vpn].physicalPage + offset;
    return physAddr; 
  }
  else 
  {
    printf("getPhyAddr received FALSE from translate\n");
    return -1;
  }
  
  
}

/*
    AddrSpace::Readfile
      Loads the code and data segments into the translated memory, instead of a position 0 like the code in the AddrSpace contrustor already does. 
    
*/
int 
AddrSpace::ReadFile(int virtAddr, OpenFile* file, int size, int fileAddr)
{
    char buffer[size]; 
    int bufferSize = file->ReadAt(buffer, size, fileAddr);
    int readSize = 0;
    int copied=  0;
    int phyAddr;

    while (bufferSize > 0) 
    {
        phyAddr = getPhyAddr(virtAddr);
    
        ASSERT(phyAddr >= 0 );
        readSize = min(PageSize,bufferSize);
        bcopy(buffer+copied, &machine->mainMemory[phyAddr],readSize);

        bufferSize -= readSize;
        copied += readSize;
        virtAddr += readSize;
    }
    
    return bufferSize;
}
//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	  machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);	

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState() 
{
    
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState() 
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
/*
    IDK 
*/

void AddrSpace::SaveReg()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++) {
	regArray[i] = machine->ReadRegister(i);
    }

}
void AddrSpace::RestoreReg()
{
    int i;
    
    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister(i, regArray[i]);
}
void AddrSpace::setPCB(pcb * input)
{
    thisPCB = input;
}

pcb* AddrSpace:: getPCB()
{
    return thisPCB;
}

int AddrSpace:: getPID() 
{
    return thisPCB->getID();
}

int AddrSpace:: getNumPages()
{
    return numPages;
}

TranslationEntry * AddrSpace:: getPageTable()
{
    return pageTable;
}
bool 
AddrSpace::check()
{
    return enoughPgs;
}
void AddrSpace:: getString(char * str, int virtAd)
{
    int i=0;
    int physAd = getPhyAddr(virtAd);
    bcopy(&(machine->mainMemory[physAd]),&str[i],1);
    
    while(str[i] != '\0' && i != 256-1)
    {
    	virtAd++;
    	i++;
    	physAd = getPhyAddr(virtAd);
        bcopy(&(machine->mainMemory[physAd]),&str[i],1);
    }

    if(i != 256-1 && str[i] != '\0')
    {
	str[i] = '\0';
    }
}

void AddrSpace:: execThread(OpenFile *executable)
{
    NoffHeader noffH;
    int oldPages = numPages;
    unsigned int i, size;
    enoughPgs = true;
    
        Lock *memLock;
        
        memLock = new Lock("memLock"); //ensure mutal exclusion 


    enoughPgs=true;

    int counter=0;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size 
			+ UserStackSize;	// we need to increase the size
						// to leave room for the stack
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;

    printf("Loaded Program: [%d] code | [%d] data | [%d] bss\n", noffH.code.size, noffH.initData.size, noffH.uninitData.size);



    //test that there are enough pages left for this process
    if(numPages > ((unsigned) memMan->getPage() + oldPages))
    {
    	//printf("Not Enough Memory for Process %d\n", this->getPID());
    	enoughPgs = false;
    	numPages = oldPages;
    	size = numPages * PageSize;
        pageIndex = 0;
    	return;
    }

    //delete current pageTable and remove pages from memory
    for(i=0; i < oldPages; i++)
    {
	    memMan->clearPage(pageTable[i].physicalPage);
    }
    delete pageTable;


    DEBUG('a', "Initializing address space, num pages %d, size %d\n", 
					numPages, size);

    memLock->Acquire();

    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++) 
    {
	pageTable[i].virtualPage = i;
	pageTable[i].physicalPage = memMan->getPage();
	pageTable[i].valid = TRUE;
	pageTable[i].use = FALSE;
	pageTable[i].dirty = FALSE;
	pageTable[i].readOnly = FALSE; 
    	//bzero(machine->mainMemory + (pageTable[i].physicalPage * 128), PageSize);
    }

    // Copy the code section into memory
    counter = 0;
    if (noffH.code.size > 0) 
    {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);

	while( counter < noffH.code.size)
	{
	    
        	executable->ReadAt(&(machine->mainMemory[getPhyAddr(noffH.code.virtualAddr+counter)]),
				1, noffH.code.inFileAddr+counter);
		counter++;
        }
    }

    // Copy the initialized data section
    counter = 0;
    if (noffH.initData.size > 0) 
    {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
			noffH.initData.virtualAddr, noffH.initData.size);
	while( counter < noffH.initData.size)
	{

        	executable->ReadAt(&(machine->mainMemory[getPhyAddr(noffH.initData.virtualAddr+counter)]),
				1, noffH.initData.inFileAddr+counter);

		counter++;
	}
    }

    //clear the registers and set machines pagetable to point to this new one
    InitRegisters();
    RestoreState();

    memLock->Release();

}