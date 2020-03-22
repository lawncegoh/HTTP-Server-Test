//server side
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <fstream>  
#include <iostream>
#include <cstring>

#define PORT 8080

std::string convertToString(char* a, int size) 
{ 
    int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
}

int main(int argc, char const *argv[])
{
    // variable instantiation for server and socket init
    int server_fd, new_socket; long valread;
    struct sockaddr_in address; // generic container for OS to read the first couple of bytes to identify address family
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    //listen then accept from pending connections
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    const char *reply;

    while(1)
    {
        printf("\n------------ Waiting for new connection ------------\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char recvBuffer[30000] = {0};
        valread = read(new_socket , recvBuffer, 30000);

        if(valread < 0)
        { 
            printf("No bytes are there to read");
        }

        int size = sizeof(recvBuffer)/sizeof(recvBuffer[0]);

        std::string converted = convertToString(recvBuffer, size);

        std::string second_token; 

        if (recvBuffer[0] == 'G')
        {
            size_t first = converted.find(' ');
            size_t end = converted.find_first_of('H');
            second_token = converted.substr(first+1, (end-2)-first);
            printf("second token: %s\n", second_token.c_str());
        }

        printf("%s\n",recvBuffer);
        //printf("%s\n", second_token.c_str());

        std::ifstream fs;
        std::string output;
        std::string line;

        printf("opening\n");
        fs.open("info.html");
        printf("opened\n");

        if (fs.is_open()) 
        {
            printf("while loop\n");
            while (!fs.eof()) 
            {
                getline(fs, line);
                output.append(line);
                output.append("\n");
            }
            output = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(output.length()) + "\n\n" + output;
            reply = output.c_str();
            // reply = (char*) malloc(output.length() * sizeof(char) + 1); 
            // printf("%s\n", output.c_str());
            // strncpy(reply, output.c_str(), output.length() - 1);
            
        } 
        else 
        {
            printf("hello");
        }
        fs.close(); 

        // std::cout << reply;

        // reply = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nOHOHOHOHO!";
        write(new_socket , reply , strlen(reply));
        printf("------------------reply message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}

 