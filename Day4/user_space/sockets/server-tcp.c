#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>

char *socket_path = "test.sock";

int main(int argc, char *argv[])
{
    int sock,listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;

    listener=socket(AF_INET, SOCK_STREAM,0);

    if (listener < 0)
    {
	perror("socket error");
	return 1;
    }

    addr.sin_family=AF_INET;
    addr.sin_port=htons(3425);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);


    if (bind(listener,(struct sockaddr*)&addr,sizeof(addr))<0)
    {
	perror("bind error");
	return 1;
    }

    listen(listener, 1);
    
    while(1) 
    {
	sock = accept(listener, NULL, NULL);
	if (sock<0)
	{
	    perror("accept error");
	    return 1;
	}
	while(1)
	{
	    bytes_read=recv(sock,buf,1024,0);
	    if (bytes_read <=0)
	    {
		break;
	    }
	    printf("Message received:%s\n",buf);
	}
	close(sock);
    }
    close(listener);
    return 0;
}



