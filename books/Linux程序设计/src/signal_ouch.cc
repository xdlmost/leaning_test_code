#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void ouch(int sig){
    printf("OUCH! - I got a signal %d\n",sig);
    signal(SIGINT,SIG_DFL);
}
int main(){
    signal(SIGINT,ouch);
    for(;;){
        printf("hehe --hehe \n");
        sleep(1);
    }
}
