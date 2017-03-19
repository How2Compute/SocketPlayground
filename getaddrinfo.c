// Standard Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Networking Headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        fprintf(stderr, "Usage: ./getaddrinfo.c [ip]\n");
    }
    
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    
    // Null out the hints and set it's internet family to AF_UNSPEC to allow ipv4 and ipv6
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    // Set it to use TCP (stream sockets)
    hints.ai_socktype = SOCK_STREAM;
    
    // If there wasen't an IP specified, connect to itself
    if(argc == 1)
    {
        hints.ai_flags = AI_PASSIVE; // Autofil the IP
        
        if((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0)
        {
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
            exit(1);
        }
    }
    // Otherwise connect to the specified server
    else
    {
        if((status = getaddrinfo(argv[1], "3490", &hints, &servinfo)) != 0)
        {
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
            exit(1);
        }
    }
    
    // Do stuff
    
    // Free the linked list
    freeaddrinfo(servinfo);
    
    
    return 0;
}