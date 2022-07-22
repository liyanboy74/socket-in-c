#include <stdio.h>

#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
#endif


int main()
{
    printf("Hello!\n");
    return 0;
}
