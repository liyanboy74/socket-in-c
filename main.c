#include <stdio.h>
#include <winsock2.h>

int main(int argc , char *argv[])
{
	WSADATA wsa;
    SOCKET s;

    struct sockaddr_in server;

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

    server.sin_addr.S_un.S_addr=inet_addr("142.250.179.110");
    server.sin_family=AF_INET;
    server.sin_port=htons(80);

    if(connect(s,(struct sockaddr *)&server,sizeof(server))<0)
    {
        puts("connect error");
        return 1;
    }

    puts("Connected");

	return 0;
}
