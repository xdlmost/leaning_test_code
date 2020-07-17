#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char message[]="Hello World";
void* thread_fun(void* args){
    for (int i =0;i<10;++i){
        printf("In thread : message is %s\n",args);
        sleep(1);
    }
    //message ="Bye!";
    strcpy(message,"Bye!");
    char* a=(char*)malloc(100);
    memset(a,'\0',100);
    strcpy(a,"thanks from thread!");
    pthread_exit(a);
}

int main(){
    pthread_t a_thread;
    int res;
    void *thread_result;
    res=pthread_create(&a_thread,NULL,thread_fun,(void*)message);
    if (0!=res){
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for thread to finish...\n");
    res=pthread_join(a_thread,&thread_result);
    if (0!=res){
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf ("Thread joined, it returned %s\n",(char*)thread_result);
    printf ("Now message is %s\n",message);
    exit(EXIT_SUCCESS);
}
