//
//  main.c
//  http_get
//
//  Created by YANGYANG on 10/18/15.
//  Copyright © 2015 YANGYANG. All rights reserved.
//
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h> //inet_addr

int main(int argc , char *argv[])
{
    char *hostname = "www.google.com";
    char ip[100];
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    
    if ( (he = gethostbyname( hostname ) ) == NULL)
    {
        //gethostbyname failed
        herror("gethostbyname");
        return 1;
    }
    
    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
    }
    int socket_desc;
    struct sockaddr_in server;
    char *message , server_reply[2000];
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );
    
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
    
    puts("Connected\n");
    
    //Send some data
    message = "GET / HTTP/1.0\r\n\r\n";
    if( send(socket_desc , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");
    
    //Receive a reply from the server
    if( recv(socket_desc, server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
    }
    puts("Reply received\n");
    puts(server_reply);
    
    return 0;
}
