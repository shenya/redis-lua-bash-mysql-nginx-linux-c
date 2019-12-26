#include <stdio.h>
#include <stdlib.h>


int array[] = {1, 2, 3, 5, 9, 12, 15};

int find_number(int *a, int length, int target)
{
    int i = 0;
    int j = length - 1;
    int sum = 0;

    while (i < j)
    {
        sum = a[i] + a[j];
	if (sum == target)
	{
		printf("a[%d], b[%d], target[%d]\n", a[i], a[j], target);
	    break;
	}
	else if (sum > target)
	{
	    j--;
	}
	else if (sum < target)
	{
	    i++;
	}
    
    
    }

	printf("length: %d\n", length);


	return 0;
}

int main(int argc, char *argv[])
{

	find_number(array, sizeof(array) / sizeof(array[0]), atoi(argv[1]));

    return 0;
}
