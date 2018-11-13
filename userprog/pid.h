#ifndef PID_H
#define PID_H 

#include "UserOpenFile.h"
class pid
{
    public:
	pid();
	int getPid();
	void removePid(int input);
    private:
	bool ids[32];
};
#endif PID_H