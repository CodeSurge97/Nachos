#ifndef UserOpenFile_H
#define UserOpenFile_H

class UserOpenFile{
    public:
        UserOpenFile(char* fileName, int index, int fID);
        ~UserOpenFile();
        int fID;
        int index;
        char *fileName;
        int offset;
};

#endif 
