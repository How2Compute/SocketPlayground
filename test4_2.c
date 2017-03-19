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
    if(argc != 4)
    {
        fprintf(stderr, "Usage: ./test4_2 ip port message\n");
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
        // Log Error!
        return -1;
    }
    else
    {
        if((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            perror("Error creating socket");
            return -2;
        }
        else
        {
            if((connect(sock_fd, res->ai_addr, res->ai_addrlen)) == -1)
            {
                perror("Error connecting");
                return -3;
            }
            else
            {
                printf("Successfully Established Connection\n\n");
                
                char *buffer;
                int len, bytes_sent;
                
                buffer = argv[3];
                len = strlen(buffer);
                
                if((bytes_sent = send(sock_fd, buffer, len, 0)) == -1)
                {
                    perror("Error sending data");
                    return -4;
                }
                else if(bytes_sent != len)
                {
                    // TODO resent other data
                    fprintf(stderr, "Data only partially transmitted\n");
                }
                else
                {
                    fprintf(stdout, "Successfully sent package!\n");
                    // TODO close the sockets
                    return 0;
                }
            }
        }
    }
}