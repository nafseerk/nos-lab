#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void rev(char str[])
{
int i;
char temp;
for(i=0;i<strlen(str)/2;i++)
	{
	temp=str[i];
	str[i]=str[strlen(str)-i-1];
	str[strlen(str)-i-1]=temp;
	}
}

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

memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=3001;
server.sin_addr.s_addr=inet_addr("127.0.0.1");
k=bind(sock_desc,(struct sockaddr *)&server,sizeof(server));
if(k==-1)
	{
	printf("Error in socket binding\n");
	exit(1);
	}

memset(&client,0,sizeof(client));
client.sin_family=AF_INET;
client.sin_port=3001;
client.sin_addr.s_addr=inet_addr("127.0.0.2");
len=sizeof(client);
while(1)
{
k=recvfrom(sock_desc,(void *)msg,sizeof(msg),0,(struct sockaddr *)&client,&len);
if(k==-1)
	{
	printf("Error in receiving\n");
	exit(1);
	}
printf("Received message: %s\n",msg);
if(strcmp(msg,"quit")==0)break;
rev(msg);
k=sendto(sock_desc,(void *)msg,sizeof(msg),0,(struct sockaddr *)&client,len);
if(k==-1)
	{
	printf("Error in sending\n");
	exit(1);
	}

}

close(sock_desc);
return 0;
}

