#include <stdio.h>
#include "test.h"
#include<stdlib.h>
#include<unistd.h>

extern int globle;

int main(){
    for(int i=0;i<100;i++){
        printf("from main :%d\n",globle);
        globle ++;
    }
    return 0;
}
