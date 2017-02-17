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
int sock_desc,temp_sock_desc;
struct sockaddr_in server,client;
socklen_t len;
int k;
int n,curr;
char ack[2],x[2];

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

printf("Established connection with client\n");

printf("Enter no of packets to send:");
scanf("%d",&n);
gets(x);
for(curr=0;curr<n;)
{
frame.num=curr;
printf("Enter packet %d contents:",curr);
gets(frame.payload);
printf("Sending packet %d...\n",curr);
sleep(1);
transmit: k=send(temp_sock_desc,(void *)&frame,sizeof(frame),0);
if(k==-1)
	{
	printf("Error in sending frame\n");
	exit(1);
	}
printf("Packet %d Sent.Waiting for acknowledgement...\n",curr);
sleep(1);
k=recv(temp_sock_desc,(void *)ack,sizeof(ack),0);
if(k==-1)
	{
	printf("Error in receiving ack\n");
	exit(1);
	}

if(ack[0]=='A')
	{
	printf("Packet %d received succesfully\n\n",curr);
	curr++;
	sleep(1);		
	}
else if(ack[0]=='R')
	{
	printf("Packet %d Lost\n",curr);
	printf("Retransmitting packet %d...\n",curr);	
	sleep(1);
	goto transmit;
	}

}

strcpy(frame.payload,"EOT");
k=send(temp_sock_desc,(void *)&frame,sizeof(frame),0);
if(k==-1)
	{
	printf("Error in sending EOT\n");
	exit(1);
	}
printf("Transmission complete\n");
close(temp_sock_desc);
close(sock_desc);
return 0;
}
