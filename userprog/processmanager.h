#ifndef ProcessManager_H
#define ProcessManager_H

#include "pcb.h"
#include "UserOpenFile.h"
#include "SysOpenFile.h"
#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

class pcb;

#define SYS_MAX_OPEN_FILES 20

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
	//UserOpenFiles* getUserOpenFiles(char *fileName);
	/*SysOpenFile* getOpenFile( char* fileName, OpenFile* openFile );
	SysOpenFile* getOpenFile( int fd );
	SysOpenFile* createNewSysFile( OpenFile* openFile, char* fileName);
	void closeFile( int id);
	int getFD( char* name );
	*/

    private:
    pcb *pcbArray[32];
	bool usage[32];
	int pcb_count;
    int currPid;
/*	BitMap* bitmap;
	SysOpenFile* sysOpenFileTable;
	BitMap* sysOpenFileMap;
	Lock** fsLock;
	Lock* bitLock;
	*/
	

};
#endif