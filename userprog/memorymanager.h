/* 

Data structures defining a memory manager -  Memory Manager is used to facilitate contiguous virtual memory. The amount of memory available to the user space is the same as the amount of physical memory.
	
	Allows kernel to allocate page frames of the simulated machine's memory for
	specific processes and to keep track of which frames are free and which
	are in use.
*/

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "synch.h"
#include "bitmap.h"
#include "system.h"


class MemoryManager 
{
	public:
		MemoryManager(int numTotalPages);
		~MemoryManager();
		int getPage();
		void clearPage(int pageId);
	private:
		BitMap *frameMap; 
		Lock *memLock;

};
#endif // MEMORYMANAGER_H