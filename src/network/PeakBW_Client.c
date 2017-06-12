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
    if(argc != 4){
        STOP("USAGE: TCPecho <server_ip> <port> <message_size>\n");
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
    if(connect(sock, (struct sockaddr*)&echoserver, sizeof(echoserver)) < 0 ){
        STOP("Failed to connect with server\n");
    }

    int message_size = atoi(argv[3]);

    uint64_t start, end, total = 0;
    char *message = (char*)malloc(message_size);
    start = rdtsc_start();
    int n  = 0;
    int t = 0;
    for(int i = 0; i < 10; i++) {
        n = send(sock, message, message_size, 0);
        if(n < 0){
            STOP("Fail to send message\n");
        }
        t+= n;
    }

    end = rdtsc_end();
    total = end - start;
    printf("Send: %d\n", n);
    double bandwidth = 10 * (t/(1024*1024*1.0)) * (2.6e9 / total);

    printf("Peak bandwidth: %f MB/s\n",  bandwidth);
    printf("Done\n");
    close(sock);
    exit(0);
}