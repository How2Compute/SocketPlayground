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
void *ConnectionAccepter(void *vargp);
void *ConnectionManager(void *vargp);

struct sockdata
{
    int fd;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    
    struct sockdata *next;
};

// Variable to keep track of connections
struct sockdata *remotes;
// Should it log?
int verbose;

int main(int argc, char *argv[])
{
    if(argc != 2 && argc != 3)
    {
        fprintf(stderr, "Usage: ./test4_2 port [-v]\n");
        return -1;
    }
    
    // If there was a third arguement, check if it was to set verbosity!
    if(argc == 3)
    {
        // Check if the flag is valid
        if(!strcmp(argv[2], "-v"))
        {
            // If it is -v set verbose to 1 AKA log out stuff
            verbose = 1;
        }
        else
        {
            // Otherwise it should not be verbose
            verbose = 0;
            fprintf(stderr, "Unknown flag: %s\n", argv[2]);
            // TODO rearrange error codes to allow for this (for example this gets -0x, connection errors get -1x etc)
            return -100;
        }
    }
    else
    {
        // As there wasent a flag specifying it should be verbose, make it quiet
        verbose = 0;
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
        fprintf(stderr, "Error fetching address: %s\n", gai_strerror(status));
        return -2;
    }
    else
    {
        if((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            perror("Error creating socket");
            return -3;
        }
        else
        {
            if(bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1)
            {
                perror("Error binding socket");
                return -4;
            }
            else
            {
                if(listen(sock_fd, CONNECTIONBACKLOG) == -1)
                {
                    perror("Error starting listener");
                    return -5;
                }
                else
                {
                    addr_len = sizeof(their_addr);
                    
                    // Fire the accepter thread
                    pthread_create(&accepterThread, NULL, ConnectionAccepter, (void *)(intptr_t)sock_fd);
                    
                    while(1)
                    {
                        // Keep the program alive
                    }
                }
            }
        }
    }
}

void *ConnectionManager(void *vargp)
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
            for(struct sockdata *connection = remotes; connection != NULL; connection = connection->next)
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


void *ConnectionAccepter(void *vargp)
{
    int sock_fd = (intptr_t)vargp;
    
    struct sockdata *connection;
    struct sockaddr_storage connectionAddr;
    socklen_t connectionLen;
    int temp_fd;
    
    printf("Accepter has started!\n");
    while(1)
    {
        
        connectionLen = sizeof(connectionAddr);
        
        if((temp_fd = accept(sock_fd, (struct sockaddr *)&connectionAddr, &connectionLen)) == -1)
        {
            perror("Error while accepting connection");
            return NULL;
        }
        else
        {
            // Allocate some space for the struct
            connection = calloc(1, sizeof(struct sockdata));
            
            // Set the data in the newly created struct
            connection->fd = temp_fd;
            connection->their_addr = connectionAddr;
            connection->addr_len = connectionLen;
            
            // Put it in the linked list of remotes
            connection->next = remotes;
            remotes = connection;
            
            // Create a thread to manage the connection
            pthread_t threadthing;
            pthread_create(&threadthing, NULL, ConnectionManager, (void *)(intptr_t)temp_fd);
            
            // Log the connection
            fprintf(stdout, "Accepted connection!\n");
        }
    }
    return NULL;
}