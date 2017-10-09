#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string>
#include <time.h>
#include <vector>

#define BUFF_SIZE 380000
#define NUM_OF_SECONDS 1
#define DELAY 2000000

using namespace std;

struct REQUESTINFO {
    char *hostName;
    int port;
    char *msg;
    char *body;
};

void runClient(REQUESTINFO *requestInfo)
{
    printf("Initializing Client ... \n");

    int    sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFF_SIZE];

    printf("Opening TCP socket ... \n");

    /* Create the TCP socket for communication */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)  {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }

    /* Get the host name (i.e. the name or IP address of the server) */
    server = gethostbyname(requestInfo->hostName);
    if (server == NULL) {
        fprintf(stderr,"Error getting host, h_errno = %d (%s) \n",
                h_errno,hstrerror(h_errno));
        return;
    }

    /* Setup the socket for network comms */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
           server->h_length);

    /* have to convert the port number to network format */
    serv_addr.sin_port = htons(requestInfo->port);

    printf("Connecting to Server ... \n");

    /* Make a connection request to the server */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)  {
        fprintf(stderr, "Error connecting to socket, errno = %d (%s) \n",
                errno, strerror(errno));
        close(sockfd);
        return;
    }

    printf("Writing to Server ... \n");

    string fullRequestMsg = string(requestInfo->msg);
    if(sizeof(requestInfo->body) > 0)
    {
        fullRequestMsg.append("\r\n\r\n");
        fullRequestMsg.append(requestInfo->body);
    }

    /* Display server response */
    printf("Sending request msg: %s\n", fullRequestMsg.c_str());


    /* Make the client request to the server */
    n = write(sockfd,requestInfo->msg,strlen(requestInfo->msg));
    if (n < 0) {
        fprintf(stderr, "Error writing to socket, errno = %d (%s) \n",
                errno, strerror(errno));
        close(sockfd);
        return;
    }

    bzero(buffer,BUFF_SIZE);

    /* Wait for response from the server */
    n = read(sockfd,buffer,BUFF_SIZE);
    if (n < 0)  {
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        close(sockfd);
        return;
    }

    /* Display server response */
    printf("%s\n",buffer);

    close(sockfd);
}

inline float getAvgElapsedTime(vector<float> elapsedTimes)
{
    int numOfTimes = elapsedTimes.size();
    float sumOfElapsedTimes = 0.0;


    for(std::vector<float>::iterator it = elapsedTimes.begin(); it != elapsedTimes.end(); ++it)
    {
        sumOfElapsedTimes += *it;
    }

    return (sumOfElapsedTimes/((float)numOfTimes));
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
      fprintf(stderr,"usage %s <hostname> <port> <msg> <body>\n", argv[0]);
       exit(0);
    }

    int port;
    char *hostName, *msg, *body;

    /* Get the host name (i.e. the name or IP address of the server) */
    hostName = argv[1];
    /* Get the service name/port number */
    port = atoi(argv[2]);
    /* Get the client request from the command line */
    msg = argv[3];
    /* Get the request body if exist */
    body = argv[4];

    struct REQUESTINFO requestInfo;
    requestInfo.hostName = hostName;
    requestInfo.port = port;
    requestInfo.msg = msg;
    requestInfo.body = body;

    clock_t time;
    float timeElapsed;
    float totalTimeElapsed = 0;
    int numberOfClients = 0;
    vector<float> timeElapsedForEachClient;
    do
    {
        numberOfClients++;
        time = clock();
        printf("Starting client ... \n");
        runClient(&requestInfo);
        time = clock() - time;
        timeElapsed = ((float)time)/CLOCKS_PER_SEC;
        printf ("Elasped time is %f seconds.\n", timeElapsed);
        timeElapsedForEachClient.push_back(timeElapsed);
        totalTimeElapsed += timeElapsed;
        usleep(DELAY);
	printf("Total time passed: %f", totalTimeElapsed);
    } while(totalTimeElapsed < NUM_OF_SECONDS);

    printf("Results......\n");
    printf("Total run time: %d seconds\n", NUM_OF_SECONDS);
    printf("Number of requests made: %d\n", numberOfClients);
    printf("Average Time Elapsed Per Client: %f\n", getAvgElapsedTime(timeElapsedForEachClient));
    printf("Number of request per seconds: %f\n", ((float)numberOfClients)/((float)NUM_OF_SECONDS));

    return 0;
}


