#include <stdio.h>

//#define ONE_UNIT=1024
/*
int mk1m(int step){
    char aa[ONE_UNIT*ONE_UNIT];
    prrintf("in %d\n",)
}
*/

int count = 0;

void foo()
{
    char array[1024] = {};
    printf("%dK\n", ++count);

    foo();
}


int main()
{
    foo();
    return 0;
    
}
