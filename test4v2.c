/*
* A relaying server for WildCard
* Alpha Version 0.0.1
* Written by Alpha-V
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Multithreading
#include <pthread.h>
#include <semaphore.h>

#define CONNECTIONBACKLOG 5
#define MAXUSERS 5

// Function prototypes
void *myThreadFun(void *vargp);

struct sockdata
{
    int fd;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    
    struct sockdata *next;
};

// TODO make this not global
/* Multithreaded sock accept */
    struct sockdata *connections;
    int numConnections;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./test4_2 port\n");
        return -1;
    }
    
    struct addrinfo hints, *res;
    int status;
    int sock_fd;
    
    int clients_connected = 0;
    int their_fd[MAXUSERS];
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    
    // The accepting thread reference
    pthread_t accepterThread;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
    {
        // TODO log error
        return -1;
    }
    else
    {
        if((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            perror("Error creating socket");
            return -2;
        }
        else
        {
            if(bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1)
            {
                perror("Error binding socket");
                return -3;
            }
            else
            {
                if(listen(sock_fd, CONNECTIONBACKLOG) == -1)
                {
                    perror("Error starting listener");
                    return -4;
                }
                else
                {
                    addr_len = sizeof(their_addr);
                    
                    // Fire the acceptor thread
                    pthread_create(&accepterThread, NULL, myThreadFun, (void *)(intptr_t)sock_fd);
                    while(1)
                    {
                        // Keep the program alive
                    }
                }
            }
        }
    }
}

void *DoRecieve(void *vargp)
{
    // TODO fix this (currently in to assre V being sent back, also in the first connect case)
    sleep(1);
    int status;
    
    char *buffer;
    int len = 1024;
                    
    buffer = malloc(sizeof(char) * len);
                    
    int sock_fd = (intptr_t)vargp;
    while (1)
    {
        if((status = recv(sock_fd, buffer, len, 0)) == -1)
        {
            perror("Error recieving data");
            return NULL;
        }
        // Check for recieved nothing case
        else if(status == 0)
        {
            
        }
        else
        {
            // Log recieve to console
            printf("Recieved:\n%s\n", buffer);
            
            // Send confirmation
            if(send(sock_fd, "V", sizeof(char) * strlen("V"), 0) == -1)
            {
                perror("Error sending validation back");
                return NULL;
            }
            
            // And relay it to the other hosts
            for(struct sockdata *connection = connections; connection != NULL; connection = connection->next)
            {
                // Don't relay it to itself tho!
                if(connection->fd != sock_fd)
                {
                    // Send/Relay the message to the client in question
                    if(send(connection->fd, buffer, len, 0) == -1)
                    {
                        perror("Error relaying information");
                    }
                }
            }
        }
    }
}


void *myThreadFun(void *vargp)
{
    int sock_fd = (intptr_t)vargp;
    
    struct sockdata *tempdata;
    struct sockaddr_storage tempstorage;
    socklen_t templen;
    int temp_fd;
    
    printf("Accepter has started!\n");
    while(1)
    {
        
        templen = sizeof(tempstorage);
        
        if((temp_fd = accept(sock_fd, (struct sockaddr *)&tempstorage, &templen)) == -1)
        {
            perror("Error while accepting connection");
            return NULL;
        }
        else
        {
            // Allocate some space for the struct
            tempdata = calloc(1, sizeof(struct sockdata));
            
            // Set the data in the newly created struct
            tempdata->fd = temp_fd;
            tempdata->their_addr = tempstorage;
            tempdata->addr_len = templen;
            
            // Put it in the linked list of connections
            tempdata->next = connections;
            connections = tempdata;
            
            // Create a thread to listen on the connection
            pthread_t threadthing;
            pthread_create(&threadthing, NULL, DoRecieve, (void *)(intptr_t)temp_fd);
            
            fprintf(stdout, "Accepted connection!\n");
        }
    }
    return NULL;
}