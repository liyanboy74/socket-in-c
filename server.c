#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CLIENTS 30
#define MAX_RECV    1024

int s_main(int argc,char**argv)
{
    WSADATA wsa;
    SOCKET master,new_socket,client_socket[MAX_CLIENTS],s;
    struct sockaddr_in server,address;
    int max_clients=MAX_CLIENTS,activity,addrlen,varlen,valread,i;
    char *message="ECHO Daemon v1.0 \r\n";

    fd_set readfds;
    char *buffer=(char*)malloc((MAX_RECV+1)*sizeof(char));

    for(i=0;i<MAX_CLIENTS;i++)
    {
        client_socket[i]=0;
    }

    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
    {
        printf("WSA Error Code : %d\n",WSAGetLastError());
        return 1;
    }

    if((master=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
    {
        printf("Socket Error Code : %d\n",WSAGetLastError());
        return 1;
    }

    server.sin_family=AF_INET;
    server.sin_addr.S_un.S_addr=INADDR_ANY;
    server.sin_port=htons(8888);

    if(bind(master,(struct sockaddr *)&server,sizeof(server))==SOCKET_ERROR)
    {
        printf("Bind Error Code : %d\n",WSAGetLastError());
        return 1;
    }

    listen(master,3);

	//Accept and incoming connection
	puts("Server running on localhost:8888\r\nWaiting for incoming connections...");

    addrlen=sizeof(struct sockaddr_in);

    while(TRUE)
    {
        FD_ZERO(&readfds);
        FD_SET(master,&readfds);
        for(i=0;i<max_clients;i++)
        {
            s=client_socket[i];
            if(s>0)
            {
                FD_SET(s,&readfds);
            }
        }

        activity=select(0,&readfds,NULL,NULL,NULL);

        if(activity==SOCKET_ERROR)
        {
            printf("select call failed with error code : %d\n" , WSAGetLastError());
			return 1;
        }

        if(FD_ISSET(master,&readfds))
        {
            if((new_socket=accept(master,(struct sockaddr*)&address,(int*)&addrlen))<0)
            {
                perror("accept");
                return 1;
            }

            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send failed");
            }

            for (i = 0; i < max_clients; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    break;
                }
            }

        }

        for(i=0;i<MAX_CLIENTS;i++)
        {
            s=client_socket[i];
            if(FD_ISSET(s,&readfds))
            {
                getpeername(s,(struct sockaddr*)&address,(int*)&addrlen);
                valread=recv(s,buffer,MAX_RECV,0);
                if(valread==SOCKET_ERROR)
                {
                    int error_code=WSAGetLastError();
                    if(error_code==WSAECONNRESET)
                    {
                        printf("Host disconnected unexpectedly , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                        closesocket(s);
                        client_socket[i]=0;
                    }
                    else
                    {
                        printf("recv failed with error code : %d\n" , error_code);
                    }
                }
                if ( valread == 0)
                {
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    closesocket( s );
                    client_socket[i] = 0;
                }
                else
                {
					buffer[valread] ='\0';
                    printf("%s:%d - %s \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port), buffer);
					send( s , buffer , valread , 0 );
                }
            }
        }
    }

    closesocket(s);
	WSACleanup();
    return 0;
}