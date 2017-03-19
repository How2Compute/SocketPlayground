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
        fprintf(stderr, "Usage: ./accept port\n");
        return -1;
    }
    
    struct addrinfo hints, *res;
    int status;
    int sock_fd;
    int their_fd;
    struct sockaddr_storage their_addr;
    // TOLEARN (Don't use sockaddrlen_t but socklen_t!)
    socklen_t addr_size;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
    {
        // Log status error using gai_something
        return -2;
    }
    else
    {
        if((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            perror("Error while creating socket");
            return -3;
        }
        else
        {
            if(bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1)
            {
                perror("Error while binding socket");
                return -4;
            }
            else
            {
                if(listen(sock_fd, CONNECTIONBACKLOG) == -1)
                {
                    perror("Error while starting listener");
                    return -5;
                }
                else
                {
                    // TOLEARN
                    addr_size = sizeof(their_addr);
                    if((their_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &addr_size)) == -1)
                    {
                        perror("Error while recieving call");
                        return -6;
                    }
                    else
                    {
                        fprintf(stdout, "Client attempted to connect!\n");
                    }
                }
            }
        }
    }
    
}