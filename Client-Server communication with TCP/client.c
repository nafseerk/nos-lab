#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>



main()
{
int sock_desc;
struct sockaddr_in server;
int k;
socklen_t len;
char msg[100];
sock_desc=socket(AF_INET,SOCK_STREAM,0);
if(sock_desc==-1)
	{
	printf("Error in socket creation\n");
	exit(1);
	}

memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=3001;
server.sin_addr.s_addr=inet_addr("127.0.0.1");
k=connect(sock_desc,(struct sockaddr *)&server,sizeof(server));
if(k==-1)
	{
	printf("Error in socket binding\n");
	exit(1);
	}

while(1)
{
printf("Enter message to reverse:");
gets(msg);
k=send(sock_desc,(void *)msg,sizeof(msg),0);
if(k==-1)
	{
	printf("Error in sending\n");
	exit(1);
	}
if(strcmp(msg,"quit")==0)break;
k=recv(sock_desc,(void *)msg,sizeof(msg),0);
if(k==-1)
	{
	printf("Error in receiving\n");
	exit(1);
	}
printf("Reversed message: %s\n",msg);
}

close(sock_desc);
return 0;
}

