#include "UserOpenFile.h"
#include "synchlist.h"
#include "filesys.h"
#include "system.h"
#include "stdio.h";


//Object class that contains fileName, index to global SysOpenFile
//table and an int offset representing the current processes 
//position in the file
UserOpenFile::UserOpenFile(char *fileName, int index, int fID){
    theOpenFile = new char[strlen(fileName)+1];
    strcpy(theOpenFile, fileName);
    fileID = fID;
    desiredIndex = index;
    offset = 0;

}