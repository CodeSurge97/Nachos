/* SysOPenFile
Class that contains pointer to the file system's openfile object as well as the systems (int)FileID and (char *) fileName for that file and the number of user processes accessing currently it.
*/


#ifndef SysOpenFile_H
#define SysOpenFile_H

#define ARRAY_SIZE 32

#include "copyright.h"
#include "openfile.h"
#include "bitmap.h"

/*
A system's open file class: Needs to contain information such as the filename, an OpenFile object, and the number of processes that have this file open. The OpenFile object is what will be used to actually read and write to this file. You can look at this in filesys/openfile.h directory where the stubs are contained. The number of process with the file open will be helpful to determine if a file really can be closed or not because other processes are still using it. It will also determine if a file needs to be opened or if it is already open.
*/

class SysOpenFile {
	public:
		SysOpenFile(int fileID, char* fileName, OpenFile *file);
		~SysOpenFile();
		int id;
		int procCount;
		char *newName;
		OpenFile* openFile;
		SysOpenFile* file[ARRAY_SIZE];
		bool OpenFileArray(char *fileName);
		bool ClearID(int i);
		int GetID();
		
	private:
		BitMap* mapFiles;
		Lock* memLock; 
};
#endif SysOpenFile_H