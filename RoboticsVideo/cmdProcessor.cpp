//
//  cmdProcessor.cpp
//  Video
//
//  Created by Robert Palmer Jr on 2/6/17.
//  Copyright © 2017 Bryson Gullett. All rights reserved.
//

#include "cmdProcessor.hpp"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>

void* server(void *context);
void* get_in_addr(struct sockaddr * sa);

cmdProcessor* cmdProcessor::pInstance = NULL;

cmdProcessor::cmdProcessor()
{
    
}

cmdProcessor::~cmdProcessor()
{
    
}

cmdProcessor* cmdProcessor::getInstance()
{
    if(pInstance == NULL)
    {
        pInstance = new cmdProcessor();
    }
    
    return pInstance;
}

void cmdProcessor::start()
{
    pthread_t thread;
    int rc;

    serverRunning = true;
    
    rc = pthread_create(&thread, NULL, server, this);
    
    if (rc){
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
    }

}

bool cmdProcessor::isRunning()
{
    return serverRunning;
}

void cmdProcessor::processMessage(char* buffer)
{
    short value;
    short frameNum;
    short preCount;
    short postCount;
    
    switch(buffer[0])
    {
        case 'r':   // re-Read config file
            std::cout << "CMD: re-read config file" << std::endl;;
            break;
            
        case 'v':   // set Value
            value = ntohs( *((short *) (buffer+1)));
            std::cout << "CMD: set value -- [" << value << "] : " << (buffer+3) << std::endl;
            break;
            
        case 's':   // Save current values
            std::cout << "CMD: save current values" << std::endl;
            break;
            
        case 'f':   // save Frames
            frameNum = ntohs( *((short *) (buffer+1)));
            preCount = ntohs( *((short *) (buffer+3)));
            postCount = ntohs( *((short *) (buffer+5)));
            std::cout << "CMD: save frames" << "start: " << frameNum-preCount << "  stop: " << frameNum+postCount << std::endl;
            break;
    }
}

void* server(void *context)
{
    
    // Variables for writing a server.
    /*
     1. Getting the address data structure.
     2. Openning a new socket.
     3. Bind to the socket.
     4. Listen to the socket.
     5. Accept Connection.
     6. Receive Data.
     7. Close Connection.
     */
    int status;
    struct addrinfo hints, *res;
    int listner;
    char processing;
    char buffer[255];
    
    cmdProcessor* cmdProc = (cmdProcessor *) context;
    
    // Before using hint you have to make sure that the data structure is empty
    memset(& hints, 0, sizeof hints);
    // Set the attribute for hint
    hints.ai_family = AF_UNSPEC; // We don't care V4 AF_INET or 6 AF_INET6
    hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM
    hints.ai_flags = AI_PASSIVE;
    
    // Fill the res data structure and make sure that the results make sense.
    status = getaddrinfo(NULL, "5801" , &hints, &res);  // will be listening on port 5801
    if(status != 0)
    {
        fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
    }
    
    // Create Socket and check if error occured afterwards
    listner = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
    if(listner < 0 )
    {
        fprintf(stderr,"socket error: %s\n",gai_strerror(status));
    }
    
    // Bind the socket to the address of my local machine and port number
    status = bind(listner, res->ai_addr, res->ai_addrlen);
    if(status < 0)
    {
        fprintf(stderr,"bind: %s\n",gai_strerror(status));
    }
    
    status = listen(listner, 10);
    if(status < 0)
    {
        fprintf(stderr,"listen: %s\n",gai_strerror(status));
    }
    
    // Free the res linked list after we are done with it
    freeaddrinfo(res);
    
    
    // We should wait now for a connection to accept
    int new_conn_fd = NULL;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    char s[INET6_ADDRSTRLEN]; // an empty string
    
    // Calculate the size of the data structure
    addr_size = sizeof client_addr;
    
    printf("I am now accepting connections ...\n");
    
    while(cmdProc->isRunning()){
        // Accept a new connection and return back the socket desciptor
        new_conn_fd = accept(listner, (struct sockaddr *) & client_addr, &addr_size);
        if(new_conn_fd < 0)
        {
            fprintf(stderr,"accept: %s\n", gai_strerror(new_conn_fd));
            continue;
        }
        
        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s);
        printf("I am now connected to %s \n",s);
        
        // process commands
        processing = true;
        do
        {
            status = read(new_conn_fd,buffer,255);
            if (status < 0)
            {
                perror("ERROR reading from socket");
                processing = false;
            }
            
            printf("Here is the message: %s\n",buffer);
            
            cmdProc->processMessage(buffer);
        } while (processing);
        
        
    }
    // Close the socket before we finish 
    close(new_conn_fd);	
    
    return NULL;
}

void * get_in_addr(struct sockaddr * sa)
{
    if(sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

