//
// Created by Andy Yan on 6/3/17.
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



    uint64_t start, end, total = 0;

    start = rdtsc_start();

    if(connect(sock, (struct sockaddr*)&echoserver, sizeof(echoserver)) < 0 ){
        STOP("Failed to connect with server\n");
    }
    end = rdtsc_end();
    total = end - start;
    printf("******************Connection Overhead Measurement**************\n");
    printf("Setup Cycles: %llu\n",  total);




    int message_size = atoi(argv[3]);

    char *message = (char*)malloc(message_size);
    start = rdtsc_start();
    int n  = 0;
    for(int i = 0; i < 10; i++) {
        n = send(sock, message, message_size, 0);
    }
    if(n < 0){
        STOP("Fail to send message\n");
    }
    end = rdtsc_end();
    total = end - start;
    printf("Send: %d\n", n);
    double bandwidth = 10*(message_size/(1024*1024*1.0)) * (2.6e9 / total);
    printf("******************Peak Bandwidth Measurement**************\n");
    printf("Peak bandwidth: %f MB/s\n",  bandwidth);




    int loop_num = 1000;
    char message2 = 'm';
    for(int i = 0; i < loop_num; i++) {
        start = rdtsc_start();
        send(sock, &message2, 1, 0);
        recv(sock, &message2, 1, 0);
        end = rdtsc_end();
        total = total + (end - start);
    }

    printf("******************Round Trip Time Measurement**************\n");
    printf("Cycles: %llu\n",  total/loop_num);




    printf("Done\n");
    close(sock);
    exit(0);
}

