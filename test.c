
    
    struct addrinfo hint, *res;
    int status;
    int socket_fd;
    
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    
    if((status = getaddrinfo(NULL, argv[1], &hint, &res)) != 0)
    {
        // Log Error
        return -2;
    }
    else
    {
        if(res == NULL)
        {
            // Log Error
            return -3;
        }
        else
        {
            if((socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) != 0)
            {
                perror("Error creating socket");
                return -4;
            }
            else
            {
                if((bind(socket_fd, res->ai_addr, res->ai_addrlen)) != 0)
                {
                    perror("Error binding socket");
                    return -5;
                }
                else
                {
                    if(listen(socket_fd, CONNECTIONBACKLOG) == 0)
                    {
                        perror("Error starting listener");
                    }
                    else
                    {
                        // Accept stuff!
                    }
                }
            }
        }
    }
    
}