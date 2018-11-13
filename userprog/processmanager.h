#ifndef ProcessManager_H
#define ProcessManager_H

#include "pcb.h"
#include "UserOpenFile.h"
class pcb;

class ProcessManager
{

    public: 
	ProcessManager();
	void assignPCB(pcb *input);
	void removePCB(int pid);
	int getNumPCB();
	bool validPID(int pid); 
	void setParentNull();
	pcb* getThisPCB(int pcbID);
	//OpenUserFiles* getOpenUserFiles(char *fileName);

    private:
    pcb *pcbArray[32];
	bool usage[32];
	int pcb_count;
    int currPid;
};
#endif