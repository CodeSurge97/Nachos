#ifndef UserOpenFile_H
#define UserOpenFile_H

class UserOpenFile{
    public:
        UserOpenFile(char* fileName, int index, int fID);
        ~UserOpenFile();
        int fileID;
        int desiredIndex;
        char *theOpenFile;
        int offset;
};
#endif UserOpenFile_H

