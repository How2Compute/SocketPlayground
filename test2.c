#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define CONNECTIONBACKLOG 5

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./test port\n");
        return -1;
    }
    
    struct addrinfo hint, *res;
    int status;
    int sock_fd;
    
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = /* !!!!!!!! TODO LEARN !!!!!!! */ AI_PASSIVE;
    
    if((status = getaddrinfo(NULL, argv[1], &hint, &res)) != 0)
    {
        // Log Error
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
                    return 5;
                }
                else
                {
                    // Ready to start accepting!
                }
            }
        }
    }
    
}