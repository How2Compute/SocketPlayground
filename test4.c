#include <stdio.h>
#include <stdlib.h>
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
        fprintf(stderr, "Usage: ./test4 port\n");
        return -1;
    }
    
    struct addrinfo hints, *res;
    int status;
    int sock_fd;
    int their_fd;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
    {
        // Log Error!
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
                    if((their_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &addr_len)) == -1)
                    {
                        perror("Error accepting connection");
                        return -6;
                    }
                    else
                    {
                        fprintf(stdout, "Successfully Established Connection!\n\n");
                        
                        char *buffer;
                        int bytes_read, len;
                        
                        len = 1024;
                        buffer = malloc(sizeof(char) * len);
                        
                        if((bytes_read = recv(their_fd, buffer, len, 0)) == -1)
                        {
                            perror("Error reading stream");
                            return -7;
                        }
                        else
                        {
                            fprintf(stdout, "Recieved:\n%s\n", buffer);
                            
                            // TODO close the connections
                            
                            return 0;
                        }
                    }
                }
            }
        }
    }
}
    