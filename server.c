#include <stdio.h>
#include <winsock2.h>

int s_main(int argc,char**argv)
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
    {
        printf("WSA Error Code : %d\n",WSAGetLastError());
        return 1;
    }

    if((s=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
    {
        printf("Socket Error Code : %d\n",WSAGetLastError());
        return 1;
    }

    server.sin_family=AF_INET;
    server.sin_addr.S_un.S_addr=INADDR_ANY;
    server.sin_port=htons(8888);

    if(bind(s,(struct sockaddr *)&server,sizeof(server))==SOCKET_ERROR)
    {
        printf("Bind Error Code : %d\n",WSAGetLastError());
        return 1;
    }
    printf("Bind OK!\n");

    closesocket(s);

    return 0;
}