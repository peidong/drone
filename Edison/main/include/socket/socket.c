#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

/* Description of data base entry for a single host.  */
struct hostent
{
  char *h_name;         /* Official name of host.  */
  char **h_aliases;     /* Alias list.  */
  int h_addrtype;       /* Host address type.  */
  int h_length;         /* Length of address.  */
  char **h_addr_list;       /* List of addresses from name server.  */
};

int main(int argc , char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    char *message;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr("128.97.89.181");
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }

    puts("Connected\n");

    //Send some data
    message = "client to server 1 time";
    if( send(socket_desc , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("client to server 1 time sent\n");

    return 0;
}
