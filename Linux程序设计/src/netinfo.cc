#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char * argv[])
{
    char *host ,**name ,**addrs;
    struct hostent *hostinfo;
    if (1==argc){
        char myname[256]={0};
        gethostname(myname,255);
        host=myname;
    }else{
        host=argv[1];
    }

    hostinfo=gethostbyname(host);
    if(!hostinfo){
        fprintf(stderr,"cannot get info for host: %s\n",host);
        exit(1);
    }
    printf("results for host %s : \n");
    printf ("Name: %s\n",hostinfo->h_name);
    printf("Aliases:");
    name=hostinfo->h_aliases;
    while(*name){
        printf(" %s",*name);
        name++;
    }
    printf("\n");

    if (hostinfo->h_addrtype !=AF_INET){
        fprintf(stderr,"not an ip host!\n");
        exit(1);
    }

    addrs=hostinfo->h_addr_list;
    while(*addrs){
        printf(" %s",inet_ntoa(*(struct in_addr *)*addrs));
        addrs++;
    }
    printf("\n");
    exit(0);
}
