#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sstream>

#define PORT 8080

int main()
{
    // create a socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        perror("Fail to create a socket.");
        return 1;
    }
    // bind to a port
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr); // convert IPv4 to IPv6

    if (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Fail to bind");
        return 1;
    }

    // listen on a port
    listen(socketfd, SOMAXCONN);
    std::stringstream ss;
    ss << PORT;
    std::cout << "Listening on port " << ss.str() <<std::endl;


    // accept the connection
    struct sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    
    // accept the connection
    int clientSocket = accept(socketfd, (sockaddr*)&client, &clientSize);

    if (clientSocket < 0){
        perror("Fail to accept");
        return 1;
    }

    close(socketfd);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client,sizeof(client),host,NI_MAXHOST,svc,NI_MAXSERV,0);

    if (result)
    {
        std::cout << host << " connected on " << svc << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;

    }
    
    char buffer[256];
 
    while (true)
    {
        memset(buffer, 0, 256);
 
        // revieve data
        int bytesReceived = recv(clientSocket, buffer, 256, 0);
        if (bytesReceived < 0)
        {
            std::cerr << "Fail to recieve " << std::endl;
            break;
        }
 
        std::cout << "Reveived: " << std::string(buffer, 0, bytesReceived) << std::endl;
 
        // echo message back to client
        send(clientSocket, buffer, bytesReceived + 1, 0);
    }
 
    // Close the socket
    close(clientSocket);
 
    return 0;

}


