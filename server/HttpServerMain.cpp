/* A simple server in the internet domain using TCP
   The port number is passed as an argument test*/
#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Request.hpp"
#include "RequestProcessor.hpp"
#include "Response.hpp"

void *cliSvr(void *arg)
{
    int   n, sockfd;
    char  buffer[256];

    bzero(buffer,256);

    sockfd = *(int *)arg;

    /* Wait for request from client */
    n = read(sockfd,buffer,255);
    if (n < 0) {
        fprintf(stderr, "Error reading from socket, errno = %d (%s)\n",
                errno, strerror(errno));
        close(sockfd);
        return NULL;
    }

    printf("Server:: Here is the message: %s\n",buffer);

    /* Process request */
    Request *request = new Request(buffer);

    /* Set up processors to process request */
    RequestProcessor *validateRequest = new ValidateRequestProcessor();
    RequestProcessor *processRequest = new ProcessRequestProcessor();
    validateRequest->setNextProcessor(processRequest);

    /* Obtain response from processed request */
    Response *response = validateRequest->process(request);

    /* Send response to the client */
    printf("Server:: Sending response: \n%s",response->getResponseString().c_str());
    n = write(sockfd,response->getResponseString().data(),response->getResponseString().size());

    if (n < 0)  {
        fprintf(stderr, "Error writing to socket, errno = %d (%s)\n",
                errno, strerror(errno));
        close(sockfd);
        return NULL;
    }

     /* Clear resources */
    delete request;
    delete validateRequest;
    delete processRequest;
    delete response;

    /* Process remainder of communication bytes and close connection */
    shutdown(sockfd, SHUT_WR);

    char recbuffer[256];
    int bytesRead;
    while((bytesRead = recv(sockfd, recbuffer, 256, 0)) > 0) {}

    if(bytesRead < 0) {
        perror("Socket read error");
    }

    close(sockfd);

    return NULL;
}


int main(int argc, char *argv[])
{
     int sockfd, clisockfd, port;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     string inputPort;

     pthread_t  tid;

     if (argc > 2) {
         fprintf(stderr,"Usage: %s <port>\n", argv[0]);
         exit(1);
     }
     else if(argc < 2) {
        printf("Start up:: No port defined. Setting default port: 8080");
        inputPort = "8080";
     }
     else {
        inputPort = string(argv[1]);
     }

     /* Open a TCP socket connection */
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return -1;
     }

     bzero((char *) &serv_addr, sizeof(serv_addr));
     port = atoi(inputPort.c_str());
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
         fprintf(stderr, "Error bind to socket, erron = %d (%s) \n",
                 errno, strerror(errno));
         return -1;
     }

     /* Setup passive listening socket for client connections */
     listen(sockfd, 5);

     /* Wait for incoming socket connection requests */
     while (1) {
         clilen = sizeof(cli_addr);
         clisockfd = accept(sockfd,
                            (struct sockaddr *) &cli_addr,
                            &clilen);

         if (clisockfd < 0) {
             fprintf(stderr, "Error accepting socket connection request, errno = %d (%s) \n",
                     errno, strerror(errno));
             break;
         }

         /* Create thread for client requests/responses */
         pthread_create(&tid, NULL, &cliSvr, (void *)&clisockfd);
     }

     close(sockfd);

     return 0;
}

