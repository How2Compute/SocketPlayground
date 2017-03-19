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
    struct sockaddr their_addr;
    socklen_t addr_len;
    int status;
    int sock_fd;
    int their_fd;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
    {
        // TODO log error
        return -2;
    }
    else
    {
        // Just to be sure, check that the first result is not NULL
        if(res == NULL)
        {
            // TODO log error
            fprintf(stderr, "Error with addrinfo (errno #0003)\n");
            return -3;
        }
        else
        {
            if((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
            {
                perror("Error creating socket");
                return -4;
            }
            else
            {
                if(bind(sock_fd, res->ai_addr, res->ai_addrlen) == -1)
                {
                    perror("Error binding socket");
                    return -5;
                }
                else
                {
                    if(listen(sock_fd, CONNECTIONBACKLOG) == -1)
                    {
                        perror("Error starting listener");
                        return -6;
                    }
                    else
                    {
                        addr_len = sizeof(their_addr);
                        if((their_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &addr_len)) == -1)
                        {
                            perror("Error accepting request");
                            return -7;
                        }
                        else
                        {
                            printf("Successfully connected!\n\n");
                            
                            char *buffer;
                            int len, readlen;
                            if((readlen = recv(their_fd, buffer, len, 0)) == -1)
                            {
                                perror("Error while recieving");
                                return -8;
                            }
                            else if(readlen == 0)
                            {
                                // Sender (probably) closed connection on us.
                                perror("Didn't recieve data");
                                return -9;
                            }
                            else
                            {
                                fprintf(stdout, "Recieved:\n%s\n", buffer);
                            }
                        }
                    }
                }
            }
        }
    }
}