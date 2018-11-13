/* SysOPenFile
	Class that contains pointer to the file system's openfile object as well as the systems (int)FileID and (char *) fileName for that file and the number of user processes accessing currently it.
*/


#ifndef SysOpenFile_H
#define SysOpenFile_H

#include "copyright.h"
#include "openfile.h"


class SysOpenFile {
	public:
		SysOpenfile(int FileID, char fileName, OpenFile *file)
		~SysOpenFile();
		int id;
		int procCount;
		char *newName;
		OpenFile* openFile;
};
#endif SysOpenFile_H