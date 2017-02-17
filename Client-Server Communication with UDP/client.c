#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


main()
{
struct sockaddr_in server,client;
int sock_desc;
int k;
char msg[100];
socklen_t len;

sock_desc=socket(AF_INET,SOCK_DGRAM,0);
if(sock_desc==-1)
	{
	printf("Error in socket creation\n");
	exit(1);
	}

memset(&client,0,sizeof(client));
client.sin_family=AF_INET;
client.sin_port=3001;
client.sin_addr.s_addr=inet_addr("127.0.0.2");
k=bind(sock_desc,(struct sockaddr *)&client,sizeof(client));
if(k==-1)
	{
	printf("Error in socket binding\n");
	exit(1);
	}


memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=3001;
server.sin_addr.s_addr=inet_addr("127.0.0.1");
len=sizeof(server);
while(1)
{
printf("Enter message to reverse:\n");
gets(msg);
k=sendto(sock_desc,(void *)msg,sizeof(msg),0,(struct sockaddr *)&server,len);
if(k==-1)
	{
	printf("Error in sending\n");
	exit(1);
	}
if(strcmp(msg,"quit")==0)break;

k=recvfrom(sock_desc,(void *)msg,sizeof(msg),0,(struct sockaddr *)&server,&len);
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

