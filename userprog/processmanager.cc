#include "processmanager.h"
#include "synch.h"
#include "bitmap.h"
#include "SysOpenFile.h"

class SysOpenFile;



#define CIN 0
#define COUT 1

ProcessManager:: ProcessManager() 
{
	int i=0;
	pcb_count = 0;
	for(i=0; i<32; i++)
	{
		usage[i]=false; 
	}
	
/*	fsLock = new Lock*[SYS_MAX_OPEN_FILES];
	bitLock = new Lock("bitLock");

	//init fs locks
	for(int i = 0 ; i< SYS_MAX_OPEN_FILES; i++){
		fsLock[i]= new Lock("fsLock");
	}
	
	sysOpenFileTable = new SysOpenFile*[SYS_MAX_OPEN_FILES];
	sysOpenFileMap = new BitMap(SYS_MAX_OPEN_FILES);

	// if we reserve these, we have to instantiate them...
	sysOpenFileMap->Mark( CIN );
	sysOpenFileMap->Mark( COUT );
*/		
}

void 
ProcessManager:: assignPCB(pcb *input)
{
	int i=0;
	for(i=0; i<32; i++)
	{
		if(usage[i] == false)
		{
		    pcbArray[i] = input;
		    usage[i]=true;
		    pcb_count++;
			currPid = i;
		    break;
		}
	} 
}

void ProcessManager:: removePCB(int pid)
{
	int i=0;
	for(i=0; i<32; i++)
	{
		if((usage[i]==true) && (pcbArray[i]->getID() == pid))
		{
			pcbArray[i] = NULL;
			usage[i] = false;
		 	pcb_count--;
			break;
		}
	 }
}
/*
int ProcessManager::getFD( char* name ){
	/// skip CIN and COUT
	for( int i = 2; i < SYS_MAX_OPEN_FILES; i++ ){
		if( sysOpenFileMap->Test( i ) ){
			// we have a file in this spot of the table
			if ( strcmp( sysOpenFileTable[i]->newName, name ) == 0 ){
				// we have a match 
				return i;
			}
		}
	}
	return -1;
}

SysOpenFile* ProcessManager::getOpenFile( char* newName, OpenFile* openFile ){
	int fd = getFD( newName );
	if( fd != -1 ){
		return sysOpenFileTable[fd];
	}
	// no open file, just create one!

	//TODO: handle full file table, if find returns -1
	fd = sysOpenFileMap->Find();
	if( fd == -1 ) return NULL;

	SysOpenFile* file = new SysOpenFile( fd, newName, openFile);
	sysOpenFileTable[fd] = file;

	return file;
}
SysOpenFile* ProcessManager::getOpenFile( int fd ){
	if( sysOpenFileMap->Test( fd ) ){
		return sysOpenFileTable[fd];
	}else{
		return NULL;
	}
}

SysOpenFile* ProcessManager::createNewSysFile( OpenFile* openFile, char* newName ){
	int index = sysOpenFileMap->Find();

	SysOpenFile* sOF = new SysOpenFile( index, newName, openFile);
	sysOpenFileTable[index] = sOF;
	return sOF;
}
void ProcessManager::closeFile( int fd ){
		fsLock[fd]->Acquire();
		if(sysOpenFileTable==NULL || sysOpenFileTable[fd]==NULL){
			//TODO this is happening still....
			DEBUG('t',"Null pointer\n");
			
			return;
		}
		sysOpenFileTable[fd]->numUsers--;
		
		if( sysOpenFileTable[fd]->numUsers == 0 ){
	// TODO: do we want to leave the file, or delete it?
				//fileSystem->Remove( sysOpenFileTable[fd]->newName );
				//delete sysOpenFileTable[fd]->openFile;
			//	delete sysOpenFileTable[fd];
	sysOpenFileTable[fd] = NULL;
				sysOpenFileMap->Clear(fd);    
		}
		fsLock[fd]->Release();
}
*/
bool ProcessManager:: validPID(int pid)
{
	int i=0;
	for(i=0; i<32; i++)
	{
		if((pcbArray[i]!=NULL) && (pcbArray[i]->getID() == pid))
		{
			currPid = i;
		    return true;
		}
	} 
	return false;
}

void ProcessManager:: setParentNull()
{
	int i;
	for(i=0; i<32; i++)
	{
		if((usage[i]==true) && (pcbArray[i]->getParent() != NULL))
			{
			pcbArray[i]->setParent(NULL);
			currPid = i;
			}
	} 

}

int ProcessManager:: getNumPCB()
{
	return pcb_count;
}

pcb* ProcessManager:: getThisPCB(int pcbID)
{
	int i;
	for(i=0; i<32; i++)
	{
		if((usage[i]==true) && (pcbArray[i]->getID() == pcbID))
		{
		    currPid = i;
			return pcbArray[i];
		    break;
		}
	}
	return NULL;
}
/*UserOpenFiles* 
ProcessManager::getUserOpenFiles(char *newName)
{
	//return a OpenUserFiles object given newName
	
}
*/