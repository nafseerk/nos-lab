#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<fcntl.h>


#define handle_error(msg) \
 do { perror(msg); exit(EXIT_FAILURE);} while(0)

struct frame_type
{
int num;
char payload[100];
}frame;

main()
{
int sock_desc,temp_sock_desc;
int k; 
struct sockaddr_in server,client;
socklen_t len;
int win_start=1,win_cur=1,win_end=3;

sock_desc=socket(AF_INET,SOCK_STREAM,0);
if(sock_desc==-1)handle_error("socket");

memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=3001;
server.sin_addr.s_addr=inet_addr("127.0.0.1");
k=bind(sock_desc,(struct sockaddr *)&server,sizeof(server));
if(k==-1)handle_error("bind");

k=listen(sock_desc,20);
if(k==-1)handle_error("listen");

memset(&client,0,sizeof(client));
temp_sock_desc=accept(sock_desc,(struct sockaddr *)&client,&len);
if(temp_sock_desc==-1)handle_error("accept");

k=recv(temp_sock_desc,(void *)&frame,sizeof(frame),0);
if(strcmp(frame.payload,"REQUEST")==0)printf("REQUEST received from client\n");

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
		printf("Received ACK for packet %d\n",frame.payload[1]-48);
		win_start++;
		win_end++;
		printf("Window advanced-%d....%d\n",win_start,win_end);
		}
	else if(frame.payload[0]=='R')
		{
		k=frame.payload[1]-48;
		printf("Received RETRANSMIT for packet %d\n",k);
		printf("Retransmitting packet %d...\n",k);
		sprintf(frame.payload,"%d",k);
		send(temp_sock_desc,(void *)&frame,sizeof(frame),0);
		printf("Packet %d sent\n",k);
		}
	sleep(2);
	}

}
	
}
