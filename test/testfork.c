#include "syscall.h"

int count = 0;
void total(){
	int i;
	for (i=0; i < 5; i++)
	{
		count++;
	}
	Exit(count);
	
}

int main() {
	int i=0;
	count++;
	Fork(total);
	Yield();
	Halt();
}