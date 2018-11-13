#include "processmanager.h"
#include "synch.h"
#include "bitmap.h"


ProcessManager:: ProcessManager() 
{
	int i=0;
	pcb_count = 0;
	for(i=0; i<32; i++)
	{
		usage[i]=false; 
	}
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
ProcessManager:: getOpenUserFiles(char *fileName)
{
	//return a OpenUserFiles object given fileName
	return //...
}