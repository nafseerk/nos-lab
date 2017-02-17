#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


struct frame_type
{
int num;
char payload[100];
}frame;

main()
{
int sock_desc;
struct sockaddr_in server;
socklen_t len;
int k;
char ack[2];

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
	printf("Error in socket connecting\n");
	exit(1);
	}

printf("Established connection with server\n");
printf("Receiving packets from server...\n");
sleep(1);

while(1)
{
k=recv(sock_desc,(void *)&frame,sizeof(frame),0);
if(k==-1)
	{
	printf("Error in receiving frame\n");
	exit(1);
	}
if(strcmp(frame.payload,"EOT")==0)break;
printf("Received packet %d\nContents: %s\n",frame.num,frame.payload);
printf("ACK : A or R?\n");
gets(ack);
if(ack[0]=='A')
	{
	printf("Packet %d valid\nAcknowledging packet %d...\n\n",frame.num,frame.num);
	sleep(1);		
	}
else if(ack[0]=='R')
	{
printf("Packet %d lost or invalid\nRequesting retransmission of packet %d...\n\n",frame.num,frame.num);
	sleep(1);		
	}
k=send(sock_desc,(void *)ack,sizeof(ack),0);
if(k==-1)
	{
	printf("Error in sending ack\n");
	exit(1);
	}


}

printf("Transmission complete\n");
close(sock_desc);
return 0;
}
