#include <stdio.h>
#include "test.h"
extern int globle;

int main(){
    for(int i=0;i<100;i++){
        printf("from main1 :%d\n",globle);
    }
    return 0;
}
