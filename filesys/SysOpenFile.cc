#include "copyright.h"
#include "openfile.h"
#include "SysOpenFile.h"

void
SysOpenFile::SysOpenfile(int fileID, char* fileName, OpenFile* file)
{
	id = FileId;
	openFile = file;
	//convert int to string and create new char obj
	newName = new char[strlen(fileID)+1];
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

SysOpenFile::OpenFileArray(char *fileName)
{
	
}