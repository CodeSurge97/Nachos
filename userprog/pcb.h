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

	
    private:
	int MAX_FILES;
	
	Thread *processThread;
	AddrSpace *AdSpace;
	int processID;
	pcb * parent_process;
	ProcessManager * children;
	int numChildren;
    int childExitValue;	
};

#endif