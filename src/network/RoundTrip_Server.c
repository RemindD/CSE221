//
// Created by Andy Yan on 5/31/17.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

void STOP(char * message){ perror(message); exit(1);}

int main(int argc, char *argv[]){
    if(argc != 2){
        STOP("USAGE: echoserver <port>\n");
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


    char message;

    while(1){
        unsigned int clientlen = sizeof(echoclient);

        if((clientsock = accept(serversock,(struct sockaddr *) &echoclient, &clientlen)) < 0){
            STOP("Failed to accept client connection");
        }

        for(int i = 0; i < loop_num; i++){
            recv(clientsock, &message, 1,0);
            send(clientsock, &message, 1,0);
        }

    }

}