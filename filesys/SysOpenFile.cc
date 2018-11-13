#include "copyright.h"
#include "openfile.h"
#include "SysOpenFile.h"

/*
This will contain the array of the system's open files and allow access to those objects. It will decide if a file already exists or if it really needs to be closed or not. You can use the bitmap (that you used in part one of the project) for managing the array of open files.
*/
void
SysOpenFile::SysOpenfile(int fileID, char* fileName, OpenFile* file)
{
	id = FileId;
	//openfile object that will be used to actually read and right to this file. 
	openFile = file;
	
	//memLock obj
	Lock* memLock = new Lock("FILE LOCK"); 
	
	//bitmap to manage files
	mapFiles = new BitMap(ARRAY_SIZE);
	
	//create new char namespace of length fileName+1
	newName = new char[strlen(fileName)+1];
	strcpy(newName, fileName);
	
	
	//# of processes using file
	//procCount = 
	//a file needs to be opeend or already open
	if(fileName exists in SysOpenArray){
		//file already open
		printf('File already open');
	}
	else{
		//look at openfile.h for sector
		Openfile(sector);
	}
	
}
int 
SysOpenFile::GetID()
{
	
}
/*
	ClearID 
		checks bitmap for unallocated bits 
*/
bool
SysOpenFile::ClearID(int i);
{
	//check if files exits
	if(mapFiles->NumClear() >= ARRAY_SIZE)
	{
		return false;
	}
	//ensure mutal exclusion 
	memLock->Acquire();
	//clear file in bitmap at index i
	mapFiles->Clear(i);
	//ensure mutal exclusion 
	memLock->Release();
	
	return true;
	
}
bool
SysOpenFile::OpenFileArray(char *fileName)
{
	int freeFiles=0;
	int usedFiles=0;
	int i;
	
	//find free spots in bitmap 
	memLock->Acquire();
	freeFiles = mapFiles->NumClear();
	memLock->Release();
	
	//It will decide if a file already exists 
	// files in use
	usedFiles = ARRAY_SIZE - freeFiles; 
	
	for(i=2; i < usedFiles; i++)
	{
		if(strcmp(file[i]->newName, fileName) == 0)
		{
			return i; 
		}
	}
	return -1;
}	
