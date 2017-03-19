#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "Usage: ./connect ip port\n");
        return -1;
    }
    
    struct addrinfo hints, *res;
    int status;
    int sock_fd;
    
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }
    else
    {
        if(res == NULL)
        {
            // TODO print error
            return -2;
        }
        else
        {
            if((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
            {
                perror("Error while creating socket");
            }
            else
            {
                if((connect(sock_fd, res->ai_addr, res->ai_addrlen)) == -1)
                {
                    perror("Error while connecting");
                }
            }
        }
    }
}