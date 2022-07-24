#include <stdio.h>
#include <winsock2.h>

int s_main(int argc,char**argv)
{
    WSADATA wsa;
    SOCKET s,new_socket;
    struct sockaddr_in server,client;
    int c;
    char *client_ip;
    int client_port;
    char *message;

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
    while((new_socket=accept(s,(struct sockaddr*)&client,&c))!=INVALID_SOCKET)
    {
        client_ip = inet_ntoa(client.sin_addr);
        client_port = ntohs(client.sin_port);

        printf("%s:%d\n",client_ip,client_port);

        message = "Hello Client , I have received your connection. But I have to go now, bye\n";
        send(new_socket,message,strlen(message),0);
    }

    if(new_socket==SOCKET_ERROR)
    {
        printf("Accept Error Code : %d\n",WSAGetLastError());
        return 1;
    }

    closesocket(s);
	WSACleanup();
    return 0;
}