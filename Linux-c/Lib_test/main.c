#include <stdio.h>

#include "my_math.h"

int main()
{
    int a = 1;
    int b = 2;
    int c = 0;

    c = add(a, b);

    printf("%d + %d = %d\n", a, b, c);

    return 0;
}
