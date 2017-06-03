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
#include <stdlib.h>

void STOP(char * message){ perror(message); exit(1);}

int main(int argc, char *argv[]){
    if(argc != 3){
        STOP("USAGE: echoserver <port> <message_size>\n");
    }
    int serversock, clientsock;
    struct sockaddr_in echoserver, echoclient;
    int loop_num = 1000;

    if((serversock  = socket(AF_INET,SOCK_STREAM,0)) < 0){
        STOP("Failed to create socket");
    }


    memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
    echoserver.sin_family = AF_INET;
    echoserver.sin_addr.s_addr = htonl(INADDR_ANY);
    echoserver.sin_port = htons(atoi(argv[1]));

    if(bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0){
        STOP("Failed to bind the server socket");
    }

    if (listen(serversock, 3) < 0) {
        STOP("Failed to listen on server socket");
    }

    puts("Listening ...  \n");


    int message_size = atoi(argv[2]);
    unsigned int clientlen = sizeof(echoclient);
    char *message = (char*)malloc(message_size+1);
    memset(message, 80, message_size + 1);

    while(1){

        if((clientsock = accept(serversock,(struct sockaddr *) &echoclient, &clientlen)) < 0){
            STOP("Failed to accept client connection");
        }


        uint64_t start, end, total = 0;

        start = rdtsc_start();
        int n  = 0;
        for(int i = 0; i < 10; i++) {
            n = recv(clientsock, message, message_size, MSG_WAITALL);
        }

        end = rdtsc_end();
        total = end - start;
        printf("Receive: %d\n", n);

        double bandwidth = 10*(message_size/(1024*1024*1.0)) * (2.6e9 / total);
        printf("******************Peak Bandwidth Measurement**************\n");
        printf("Peak bandwidth: %f MB/s\n",  bandwidth);
        //printf("Done\n");
        //close(clientsock);



        for(int i = 0; i < loop_num; i++){
            recv(clientsock, &message, 1,0);
            send(clientsock, &message, 1,0);
        }



        start = rdtsc_start();
        close(clientsock);
        end = rdtsc_end();
        total = end - start;
        printf("******************Connection Overhead Measurement**************\n");
        printf("Tear-down Cycles: %llu\n", total);
        printf("Done\n");
        close(serversock);


        exit(0);

    }

}