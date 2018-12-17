#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>

#define TEMP_FILE "/tmp/filelock_gfi89rew"
#define C_FLAG (O_RDWR|O_CREAT)
#define L_FLAG (C_FLAG|O_EXCL)

int main (){
    int fd =-1;
    printf("test file without O_EXCL\n");
    printf("Create one file without O_EXCL\n");
    fd = open(TEMP_FILE,C_FLAG,0444);
    if (-1==fd){
        printf("%d - Lock already present\n",getpid());
    }else{
        printf("%d -Get lock file \n",getpid());
        printf("%d -Close file \n",getpid());
        close(fd);
        printf("%d -Unlink lock file \n",getpid());
        unlink(TEMP_FILE);
    }
    printf("Create two files without O_EXCL\n");
    fd = open(TEMP_FILE,C_FLAG,0444);
    if (-1==fd){
        printf("%d - Lock already present\n",getpid());
    }else{
        printf("%d -Get lock file \n",getpid());
        printf("%d -Close file \n",getpid());
        close(fd);
        printf("%d - attend to get another\n",getpid());
        int fd2 = open(TEMP_FILE,C_FLAG,0444);
        if (-1==fd2){
            printf("%d - another file fail\n",getpid());
        }else{
            printf("%d - another file success\n");
            printf("%d -Close file \n",getpid());
            close(fd2);
        }
        printf("%d -Unlink lock file \n",getpid());
        unlink(TEMP_FILE);
    }
    printf("\n\ntest file with O_EXCL\n");
    printf("Create one file with O_EXCL\n");
    fd = open(TEMP_FILE,L_FLAG,0444);
    if (-1==fd){
        printf("%d - Lock already present\n",getpid());
    }else{
        printf("%d -Get lock file \n",getpid());
        printf("%d -Close file \n",getpid());
        close(fd);
        printf("%d -Unlink lock file \n",getpid());
        unlink(TEMP_FILE);
    }
    printf("Create two files with O_EXCL\n");
    fd = open(TEMP_FILE,L_FLAG,0444);
    if (-1==fd){
        printf("%d - Lock already present\n",getpid());
    }else{
        printf("%d -Get lock file \n",getpid());
        printf("%d -Close file \n",getpid());
        close(fd);
        printf("%d - attend to get another\n",getpid());
        int fd2 = open(TEMP_FILE,L_FLAG,0444);
        if (-1==fd2){
            printf("%d - another file fail\n",getpid());
        }else{
            printf("%d - another file success\n");
            printf("%d -Close file \n",getpid());
            close(fd2);
        }

        printf("%d -Unlink lock file \n",getpid());
        unlink(TEMP_FILE);
    }}
