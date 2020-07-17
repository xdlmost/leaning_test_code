#include<termios.h>
#include<stdio.h>
#include<stdlib.h>

#define PASSWORD_LEN 8

int main(){
    struct termios initialsettings,newsettings;
    char password[PASSWORD_LEN+1];
    tcgetattr(fileno(stdin),&initialsettings);
    newsettings=initialsettings;
    newsettings.c_lflag&=~ECHO;
    printf("Enter password: ");
    if(0!=tcsetattr(fileno(stdin),TCSAFLUSH,&newsettings)){
        fprintf(stderr,"Could not set attributes\n");
    }else{
        fgets(password,PASSWORD_LEN,stdin);
        tcsetattr(fileno(stdin),TCSANOW,&initialsettings);
        fprintf(stdout,"\nYou entered %s\n",password);
    }
    exit(0);
}
