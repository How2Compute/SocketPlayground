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
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int sock_fd;
    int client_fd;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    
    if((getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
    {
        // TODO error
        return -1;
    }
    else
    {
        if(res == NULL)
        {
            fprintf(stderr, "Could not get first address element. (#0002)\n");
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
                if((bind(sock_fd, res->ai_addr, res->ai_addrlen)) == -1)
                {
                    perror("Error while binding socket");
                    return -4;
                }
                else
                {
                    if(listen(sock_fd, CONNECTIONBACKLOG) == -1)
                    {
                        perror("Error while attemting to start listener");
                        return -5;
                    }
                    else
                    {
                        if((client_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &addr_size)) == -1)
                        {
                            perror("Error while attempting to accept TCP call");
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
    
}