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
	//convert int to string and create new char obj
	newName = new char[strlen(fileID)+1];
	strcpy(newName, fileName);
}