#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    FILE* read_fp;
    char buffer[BUFSIZ+1];
    int chars_read;
    memset(buffer,'\0',sizeof(buffer));
    read_fp=popen("uname -a","r");
    if(NULL==read_fp){
        perror("Open pipe fail\n");
    }else{
        chars_read=fread(buffer,sizeof(buffer),BUFSIZ,read_fp);
//        if(chars_read>0){
            printf("Output was :- \n%s\n",buffer);
//        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
}
