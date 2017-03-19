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
    if(argc != 4)
    {
        fprintf(stderr, "Usage: ./accept server port message\n");
        return -1;
    }
    
    struct addrinfo hints, *res;
    int status;
    int sock_fd;
    int their_fd;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0)
    {
        // TODO error log
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
                // Connection to server setup!
                fprintf(stdout, "Connection Successfully Established!\n\n");
                
                int len, bytes_sent;
                len = strlen(argv[3]);
                if((bytes_sent = send(sock_fd, argv[3], len, 0)) == 1)
                {
                    perror("Error sending message");
                    return -4;
                }
                else if(bytes_sent != len)
                {
                    // TODO send another packet with more of the text?
                }
                else
                {
                    fprintf(stdout, "Successfully sent package!\n");
                }
            }
        }
    }
}