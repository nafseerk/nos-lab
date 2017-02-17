#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<fcntl.h>


struct frame_type
{
int num;
char payload[100];
}frame;

main()
{
int sock_desc;
int k;
struct sockaddr_in server;
int cur=1;
int firstTime=1;
sock_desc=socket(AF_INET,SOCK_STREAM,0);
if(sock_desc==-1)
	{
	printf("Error creating socket\n");
	exit(1);
	}

memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=3001;
server.sin_addr.s_addr=inet_addr("127.0.0.1");
k=connect(sock_desc,(struct sockaddr *)&server,sizeof(server));
if(k==-1)
	{
	printf("Error in connecting\n");
	exit(1);
	}

strcpy(frame.payload,"REQUEST");
printf("Sending request to server...\n");
sleep(1);
k=send(sock_desc,(void *)&frame,sizeof(frame),0);
if(k==-1)
	{
	printf("Error in sending\n");
	exit(1);
	}


while(cur<=9)
{
recv(sock_desc,(void *)&frame,sizeof(frame),0);
sscanf(frame.payload,"%d",&cur);
printf("Got Packet %d\n",cur);
if(cur==3&&firstTime)
	{
	printf("Packet %d corrupted\n",cur);
	printf("Sending retransmission request for packet %d\n",cur);
	sprintf(frame.payload,"R%d",cur);
	send(sock_desc,(void *)&frame,sizeof(frame),0);
	firstTime=0;
	}
else
	{
	printf("Sending ACK for packet %d\n",cur);
	sprintf(frame.payload,"A%d",cur);
	send(sock_desc,(void *)&frame,sizeof(frame),0);
	}
sleep(2);
}

close(sock_desc);
return 0;	

}

