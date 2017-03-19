#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define CONNECTIONQUESIZE 5

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./listen port\n");
        return -1;
    }
    
    struct addrinfo hints, *res;
    int status;
    int sock_fd;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }
    else
    {
        if(res != NULL)
        {
            if((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) != 0)
            {
                perror("Error creating socket");
                return -3;
            }
            else
            {
                if((bind(sock_fd, res->ai_addr, res->ai_addrlen)) != 0)
                {
                    perror("Error binding socket");
                    return -4;
                }
                else
                {
                    if((listen(sock_fd, CONNECTIONQUESIZE)) != 0)
                    {
                        perror("Error while attempting to start listener");
                        return -5;
                    }
                    else
                    {
                        // Accept connections!
                    }
                }
            }
        }
        else
        {
            // TODO log error
            return -2;
        }
    }
    
}