#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

char *socket_path = "test.sock";

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    char msg[]="The Matrix has you";
    char buf[sizeof(msg)];
    int sock, rc;

    sock=socket(AF_INET,SOCK_STREAM,0);
    if (sock<0)
    {
	perror("socket error");
	return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port=htons(3425);
    //addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    inet_aton("192.168.99.18", &addr.sin_addr.s_addr);

    if(connect(sock,(struct sockaddr *)&addr,sizeof(addr))<0)
    {
	perror("connect error");
	return 1;
    }


    if (rc = send(sock,msg,sizeof(msg),0))
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
    close(sock);
    return 0;
}