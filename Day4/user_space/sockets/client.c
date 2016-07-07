#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

char *socket_path = "test.sock";

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    char buf[]="The matrix has you";
    int fd, cl, rc;

    if (argc > 1)
    {
    socket_path = argv[1];
    }

    if ((fd=socket(AF_UNIX,SOCK_STREAM,0))==-1)
    {
    perror("socket error");
    return 1;
    }

    memset(&addr,0,sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    
    if (connect(fd,(struct sockaddr*)&addr,sizeof(addr))==-1)
    {
	perror("connect error");
	return 1;
    }
    if (rc = send(fd,buf,sizeof(buf),0))
    {
	if(rc>0)
	{
	    printf("write\n");
	}
	else
	{
	    perror("write error");
	    return 1;
	}
    }
    return 0;
}