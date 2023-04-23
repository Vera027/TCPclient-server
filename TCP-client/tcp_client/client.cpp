#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


int main()
{
    //create a socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        return 1;
    }

    //create serv_addr structure for server 
    int port = 8080;
    std::string ip = "127.0.0.1";

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

    //connect to the server
    int connectSevr = connect(socketfd, (sockaddr*)&serv_addr, sizeof(sockaddr_in));
    if (connectSevr < 0)
    {
        return 1;
    }

    char buffer[256];
    std::string userInput;
    
    while (true)
    {
        //enter lines of text
        std::cout << ">";
        std::getline(std::cin, userInput);   

        //send to server
        int sendResult = send(socketfd, userInput.c_str(), userInput.size()+1, 0);
        if (sendResult < 0)
        {
            std::cout << "Fail to send " << std::endl;
            continue;
        }
        //sleep for 1 seconds
        usleep(6000000);

        //read data from socket and display
        memset(buffer, 0 , 256);
        int bytesReceived = recv(socketfd, buffer, 256, 0);
        if (bytesReceived < 0)
        {
            std::cout << "Error!" << std::endl;
        }
        else
        {
            std::cout << "SERVER> " << std:: string(buffer, bytesReceived) << std::endl;
        }         
    } 
  
    //close the socket
    close(socketfd);


    return 0;
          

}

