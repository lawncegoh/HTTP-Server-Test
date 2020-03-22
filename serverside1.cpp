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
#include <ios>
#include <pthread.h>
#include <thread>
using namespace std;

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

 // variable instantiation for server and socket init
int server_fd;
struct sockaddr_in address; // generic container for OS to read the first couple of bytes to identify address family
int addrlen = sizeof(address);

void *task(void* arg)
{
    std::thread::id threadId = std::this_thread::get_id();   
    long valread;
    int new_socket = *(int*) arg; 

    // if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    // {
    //     std::cout << "accept < 0";
    //     perror("In accept");
    //     exit(EXIT_FAILURE);
    // }
    const char *reply;
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
        second_token = converted.substr(first+2, (end-2)-first);
    }

    std::ifstream fs;
    std::string output;
    std::string line;

    printf("retrieving %s\n", second_token.c_str());

    char * cstr = new char [second_token.length()+1];
    std::strcpy (cstr, second_token.c_str());
    char * p = std::strtok(cstr," ");

    fs.open(p, ios::out);

    if (fs.is_open()) 
    {
        while (!fs.eof()) 
        {
            getline(fs, line);
            output.append(line);
            output.append("\n");
        }
        output = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(output.length()) + "\n\n" + output;
        reply = output.c_str();
    } 
    else 
    {
        output = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length:12\nFile cannot be opened! :(";
    }
    fs.close(); 

    write(new_socket , reply , strlen(reply));
    
    close(new_socket);
    return 0;
}

int main(int argc, char const *argv[])
{
    // // variable instantiation for server and socket init
    // int server_fd;
    // struct sockaddr_in address; // generic container for OS to read the first couple of bytes to identify address family
    // int addrlen = sizeof(address);

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

    
    pthread_t thread1, thread2;
    void *ret_join;

    while(1)
    {
        printf("\n------------ Waiting for new connection ------------\n\n");
        
        std::thread::id threadId = std::this_thread::get_id();   

        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            std::cout << "accept < 0";
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        pthread_create(&thread1, NULL, task, (void*) &new_socket);

        // pthread_join(thread1, NULL);
        
        printf("------------------reply message sent-------------------\n");
    }
    return 0;
}

