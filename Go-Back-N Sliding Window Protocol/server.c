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
int sock_desc,temp_sock_desc;
int k;
socklen_t len;
struct sockaddr_in server,client;
int win_start=1,win_end=3,win_cur=1;
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
k=bind(sock_desc,(struct sockaddr *)&server,sizeof(server));
if(k==-1)
	{
	printf("Error in socket binding: %d\n",errno);
	perror("bind");
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

k=recv(temp_sock_desc,(void *)&frame,sizeof(frame),0);
if(k==-1)
	{
	printf("Error in receiving\n");
	exit(1);
	}
if(strcmp(frame.payload,"REQUEST")==0)printf("Received request from client\n");
sleep(1);
fcntl(temp_sock_desc,F_SETFL,O_NONBLOCK);

while(1)
{
if(win_cur<=win_end && win_cur<=9)
	{
	sprintf(frame.payload,"%d",win_cur);
	send(temp_sock_desc,(void *)&frame,sizeof(frame),0);
	printf("Packet %d sent\n",win_cur);
	win_cur++;	
	}
if(recv(temp_sock_desc,(void *)&frame,sizeof(frame),0)!=-1)
{
if(frame.payload[0]=='A')
	{
	printf("Received ACK for Packet %d\n.Advancing window\n\n",frame.payload[1]-48);
	if(frame.payload[1]-48==9)break;
	win_start++;
	win_end++;
	}
else if(frame.payload[0]=='R')
	{
	printf("Received RETRANSMIT for Packet %d\n.Resending packet\n\n",frame.payload[1]-48);
	win_cur=frame.payload[1]-48;
	}
sleep(2);
}
}

close(sock_desc);
close(temp_sock_desc);
return 0;	

}

