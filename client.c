#include <stdio.h>
#include <winsock2.h>

#define REPLAY_BUFFER_MAX_SIZE 2000

int c_main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message,server_reply[REPLAY_BUFFER_MAX_SIZE];
    int recv_size,i;
    struct hostent *he;
    struct in_addr **addr_list;
    char ip[100];

    char *hostname="worldtimeapi.org";
    message="GET /api/timezone/Asia/Tehran HTTP/1.1\r\n\r\n";

    server.sin_family=AF_INET;
    server.sin_port=htons(80);
    //server.sin_addr.S_un.S_addr=inet_addr("213.188.196.246");

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    if((he=gethostbyname(hostname))==NULL)
    {
        printf("gethostbyname failed : %d",WSAGetLastError());
        return 1;
    }

	//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list=(struct in_addr **) he->h_addr_list;
    for(i=0;addr_list[i]!=NULL;i++)
    {
        strcpy(ip,inet_ntoa(*addr_list[i]));
    }
    printf("%s resolved to : %s\n" , hostname , ip);

    server.sin_addr.S_un.S_addr=inet_addr(ip);

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

    if((recv_size=recv(s,server_reply,REPLAY_BUFFER_MAX_SIZE,0))==SOCKET_ERROR)
    {
        puts("recv failed");
        return 1;
    }
    puts("Replay received");

    //Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);

    closesocket(s);
    WSACleanup();

    return 0;
}
