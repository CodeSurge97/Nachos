#include "synch.h"
#include "bitmap.h"
#include "system.h"
#include "machine.h"
#include "memorymanager.h"
/*
memorymanager 
Routines to manage memory  

Allows kernel to allocate page frames of the simulated machine's memory for
specific processes and to keep track of which frames are free and which
are in use.
*/
		
int allocFrames;
int numFrames;

MemoryManager::MemoryManager(int numTotalPages)
{

	numFrames = numTotalPages; // phys mem is divided into same size blocks as the pages
	allocFrames =0; //allocated frames in phys mem are initialized to zero
	frameMap = new BitMap(numTotalPages); //bit map - Each bit represents whether         
	                                     //the frame is in use or free.
	memLock = new Lock("memLock"); //ensure mutal exclusion 
	
	
}
/* 
*/
MemoryManager::~MemoryManager()
{
	delete frameMap;
	delete memLock;
}
/* 
	getPage() allocates the first clear page 
*/
int
MemoryManager::getPage()
{
	//return -1 if no pages are clear. 
	//If # of allocated frames is >= number of frames,
	//then no free frames are available. 
	if(allocFrames >= numFrames )
	{
		return -1;
	}
	// using mutual exclusion, allocate first clear page
	memLock->Acquire();
	int frameNum = frameMap->Find();
	//frameMap->Print();
	//intcrement freeFrames if available 
	if(frameNum >= 0)
	{
		allocFrames++;
	}
	memLock->Release();
	
	return frameNum;
}
/* 
	clearPage() takes the index of a frame and frees it 
*/
void
MemoryManager::clearPage(int i)
{
	//ensure mutual exclusion
	memLock->Acquire();
	//Clear frame at index i 
	frameMap->Clear(i);
	// decrement the free frames 
	allocFrames--;
	memLock->Release();
}
