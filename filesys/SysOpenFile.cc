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
}