#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>


#define handle_error(msg) \
 do { perror(msg); exit(EXIT_FAILURE);} while(0)

struct frame_type
{
int num;
char payload[100];
}frame;


main()
{
int sock_desc;
socklen_t len;
int k;
int firstTime;
int curPacket=1;
struct sockaddr_in client;

sock_desc=socket(AF_INET,SOCK_STREAM,0);
if(sock_desc==-1)handle_error("socket");
	
memset(&client,0,sizeof(client));
client.sin_family=AF_INET;
client.sin_port=3001;
client.sin_addr.s_addr=inet_addr("127.0.0.1");
k=connect(sock_desc,(struct sockaddr *)&client,sizeof(client));
if(k==-1)handle_error("connect");

strcpy(frame.payload,"REQUEST");
send(sock_desc,(void *)&frame,100,0);

firstTime=1;
while(curPacket!=10)
{
recv(sock_desc,(void *)&frame,100,0);
sscanf(frame.payload,"%d",&curPacket);
printf("Got Packet %d\n",curPacket);

if(curPacket==3 && firstTime)
	{
	printf("Packet %d Corrupted\n",curPacket);
	printf("Sending Retransmission request for packet %d\n",curPacket);
	sprintf(frame.payload,"R%d",curPacket);
	send(sock_desc,(void *)&frame,100,0);
	firstTime=0;
	}	
else
	{
	printf("Sending ACK for packet %d\n",curPacket);
	sprintf(frame.payload,"A%d",curPacket);
	send(sock_desc,(void *)&frame,100,0);
	}
sleep(1);
}
close(sock_desc);
}
