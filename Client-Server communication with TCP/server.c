#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>


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
int sock_desc,temp_sock_desc;
struct sockaddr_in server,client;
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
k=bind(sock_desc,(struct sockaddr *)&server,sizeof(server));
if(k==-1)
	{
	printf("Error in socket binding\n");
	exit(1);
	}

k=listen(sock_desc,20);
if(k==-1)
	{
	printf("Error in socket listening\n");
	exit(1);
	}

memset(&client,0,sizeof(client));
len=sizeof(client);
temp_sock_desc=accept(sock_desc,(struct sockaddr *)&client,&len);
if(temp_sock_desc==-1)
	{
	printf("Error in accepting connection\n");
	exit(1);
	}

while(1)
{
k=recv(temp_sock_desc,(void *)msg,sizeof(msg),0);
if(k==-1)
	{
	printf("Error in receiving\n");
	exit(1);
	}
printf("Received message: %s\n",msg);
if(strcmp(msg,"quit")==0)break;
rev(msg);
k=send(temp_sock_desc,(void *)msg,sizeof(msg),0);
if(k==-1)
	{
	printf("Error in sending\n");
	exit(1);
	}
}

close(temp_sock_desc);
return 0;
}

