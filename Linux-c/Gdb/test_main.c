#include <stdio.h>
#include <unistd.h>

#include "method.h"


int show_str(const char *str)
{
	printf("str:%s\n", str);
	int i = 0;

	for (i = 0; i < 5; i++)
	{
	    printf("i:%d\n", i);
	}

    return 0;
}

int main(void)
{
	int i = 0;
	int c = 0;


	show_str("hello_world");

	while (1)
	{
	
	c = my_add(1, 2);
	printf("c:%d\n", c);
	sleep(5);

        }

    return 0;
}
