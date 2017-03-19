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
        fprintf(stderr, "Usage: ./socket ip\n");
        return 1;
    }
    
    struct addrinfo hints, *res, *p;
    int status;
    
    int s;
    
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if((status = getaddrinfo(argv[1], "80", &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    
    // Just use the first result, if it is not null
    if(res != NULL)
    {
        if((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            fprintf(stderr, "Something went wrong while creating a socket!\n");
            return 3;
        }
    }
    
    return 0;
}