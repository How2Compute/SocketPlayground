#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <cs50.h>

#define CONNECTIONBACKLOG 5

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        fprintf(stderr, "Usage: ./test4_2v2 ip port message\n");
        return -1;
    }
    
    struct addrinfo hints, *res;
    int status;
    int sock_fd;
    int serv_fd;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0)
    {
        // TODO log an error (not sure if this works)
        perror("Error getting server information");
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
            if((connect(sock_fd, res->ai_addr, res->ai_addrlen)) == -1)
            {
                perror("Error connecting to server");
                return -4;
            }
            else
            {
                fprintf(stdout, "Connection to server established!\n\n");
                
                if(send(sock_fd, argv[3], sizeof(argv[3]), 0) == -1)
                {
                    perror("Error sending data");
                    close(serv_fd);
                    close(sock_fd);
                    return -5;
                }
                else
                {
                    fprintf(stdout, "Successfully sent data, waiting for a reply...");
                }
                
                char *buffer;
                int len = 1024;
                int bytes_read = 0;
                
                buffer = malloc(sizeof(char) * len);
                while(1)
                {
                if((bytes_read = recv(sock_fd, buffer, len, 0)) == -1)
                {
                    perror("Error recieving conformation");
                    return -6;
                }
                // If there wasen't any new data do nothing
                else if(bytes_read == 0)
                {
                    
                }
                else
                {
                    fprintf(stdout, ">%s\n", buffer);
                    /*
                    if(!strcmp(buffer, "ack!"))
                    {
                        fprintf(stderr, "Recieved invalid response from server!");
                        free(buffer);
                        close(serv_fd);
                        close(sock_fd);
                        
                        return -7;
                    }
                    else
                    {
                        fprintf(stdout, "Gotten response %s back\n", buffer);
                            
                        fprintf(stdout, "Done! (recieved ack!)");
                        
                        free(buffer);
                        close(serv_fd);
                        close(sock_fd);
                        
                        return 0;
                    }
                    */
                }
                
            }
            }
        }
    }
}