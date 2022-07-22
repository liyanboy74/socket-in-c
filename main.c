#include <stdio.h>
#include <winsock2.h>

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message,server_reply[2000];
    int recv_size;

    server.sin_addr.S_un.S_addr=inet_addr("213.188.196.246");
    server.sin_family=AF_INET;
    server.sin_port=htons(80);

    message="GET /api/timezone/Asia/Tehran HTTP/1.1\r\n\r\n";

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    if((s=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
    {
        printf("Could not create socket : %d",WSAGetLastError());
    }

    printf("Socket created.\n");

    if(connect(s,(struct sockaddr *)&server,sizeof(server))<0)
    {
        puts("connect error");
        return 1;
    }

    puts("Connected");


    if(send(s,message,strlen(message),0)<0)
    {
        puts("Send failed");
        return 0;
    }
    puts("Data Send");

    if((recv_size=recv(s,server_reply,2000,0))==SOCKET_ERROR)
    {
        puts("recv failed");
        return 1;
    }
    puts("Replay received");

    //Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);

    return 0;
}
