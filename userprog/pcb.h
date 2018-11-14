#ifndef PCB_H
#define PCB_H 

#include "thread.h"
#include "synch.h"
#include "addrspace.h"
#include "processmanager.h"
#include "UserOpenFile.h"

class Thread;
class Lock;
class AddrSpace;
class ProcessManager;

#define ARRAY_SIZE 21

class pcb {
    public:
	pcb(Thread *input);
	~pcb();
	int  getID();
	AddrSpace* getAddrSpace();
	void setAddrSpace(AddrSpace* input);
	void setParent(pcb * p);
	void addChild(pcb * c);
	void removeChild(int idNum);
	void setThread(Thread *input);
	bool checkForChild(int id);
	pcb * getParent();
	int numberChildren();
	void setChildExitValue(int input);
	int getChildExitValue();
	void setParentsNull();
	Thread* returnThread();
	//modify PCB to contain array of OpenUserFiles
//	UserOpenFile *openUserFiles[ARRAY_SIZE];

	
    private:
	Thread *processThread;
	AddrSpace *AdSpace;
	int processID;
	pcb * parent_process;
	ProcessManager * children;
	int numChildren;
    int childExitValue;	
};

#endif