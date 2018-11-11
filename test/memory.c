#include "syscall.h"

int
main()
{
	int array[20], i, sum=0;
	
	for( i=0; i<20; i++)
	{
		array[i]=i;
	}
	for(i=0; i<20; i++)
	{
		sum+=array[i];
	}
	Exit(0);
	return(0);
}