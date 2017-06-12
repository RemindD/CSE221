//
// Created by Andy Yan on 6/1/17.
//

#include "rdtsc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void STOP(char * message){ perror(message); exit(1);}

int main(int argc, char *argv[]){
    if(argc != 3){
        STOP("USAGE: TCPecho <server_ip>  <port>\n");
    }
    struct sockaddr_in echoserver;
    int sock;

    memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
    echoserver.sin_family = AF_INET;
    echoserver.sin_addr.s_addr = inet_addr(argv[1]);
    echoserver.sin_port = htons(atoi(argv[2]));

    if((sock = socket(AF_INET,SOCK_STREAM,0)) <  0){
        STOP("Failed to create socket\n");
    }
    uint64_t start, end, total = 0;

    start = rdtsc_start();

    if(connect(sock, (struct sockaddr*)&echoserver, sizeof(echoserver)) < 0 ){
        STOP("Failed to connect with server\n");
    }
    end = rdtsc_end();
    total = end - start;
    printf("Setup Cycles: %f\n",  total/2.6/1000000);
    printf("Done\n");
    //close(sock);
    //exit(0);
}