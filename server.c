#include <stdio.h>
#include <winsock2.h>

int s_main(int argc,char**argv)
{
    WSADATA wsa;
    SOCKET s,new_socket;
    struct sockaddr_in server,client;
    int c;

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

    listen(s,3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

    c=sizeof(struct sockaddr_in);
    new_socket=accept(s,(struct sockaddr*)&client,&c);
    if(new_socket==SOCKET_ERROR)
    {
        printf("Accept Error Code : %d\n",WSAGetLastError());
        return 1;
    }

    puts("Connection accepted");

	closesocket(s);
	WSACleanup();
    return 0;
}