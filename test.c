#include <stdio.h>
#include <stdlib.h>

int
main()
{
	
	int array[2][3] = {
				{0,1,2},
				{3,4,5}
			} ;
	int **arrPtr = &(&array);
	printf("%d\n", arrPtr[1][1]);
}
