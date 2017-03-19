#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./bind port\n");
        return -1;
    }
    
    struct addrinfo hints, *res;
    int status;
    int s;
    
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -2;
    }
    
    // Attempt to use the first result
    if(res == NULL)
    {
        fprintf(stderr, "Invalid result!\n");
        return -3;
    }
    else
    {
        if((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            perror("Error while creating socket socket ");
            return -4;
        }
        else
        {
            if(bind(s, res->ai_addr, res->ai_addrlen) == -1)
            {
                perror("Error while binding socket ");
                return -5;
            }
        }
    }
    
    
}