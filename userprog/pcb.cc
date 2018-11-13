#include "pcb.h"
#include "system.h"

//@Liz Does this already take care of the first part of #3 on Guide?
pcb::pcb(Thread *input)
{
	int i;

    processID = pid_manager->getPid();
	parent_process = NULL;
	processThread = input;
	children = new ProcessManager();
	childExitValue=0;
	MAX_FILES = 21;

}

pcb:: ~pcb()
{
	delete processThread; 
}

int pcb:: getID()
{
	return processID;
}

AddrSpace* pcb:: getAddrSpace()
{
	return AdSpace;
}

void pcb:: setAddrSpace(AddrSpace* input)
{
	AdSpace = input;

}

void pcb::setParent(pcb * p) 
{
	parent_process = p;
}

pcb * pcb::getParent() 
{
	return parent_process;
}

void pcb::addChild(pcb * c) 
{

	children->assignPCB(c);
	numChildren++;

}

void pcb:: removeChild(int idNum)
{
	children->removePCB(idNum);
	numChildren--;
}


void pcb:: setThread(Thread *input)
{
	processThread = input;
}

int pcb:: numberChildren()
{
	return numChildren;
}

bool pcb:: checkForChild(int id)
{
	return children->validPID(id);
}

int pcb:: getChildExitValue()
{
	return childExitValue;
}

void pcb:: setChildExitValue(int input)
{
	childExitValue = input;
}

void pcb:: setParentsNull()
{
	children->setParentNull();
}

Thread* pcb:: returnThread()
{
	return processThread;
}
