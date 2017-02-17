#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>

#define  handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); }while(0)

struct email_type
{
char to[20];
char from[20];
char subject[30];
char body[100];
};

main()
{
struct sockaddr_in server,client;
int sock_desc;
int i,j,k;
struct email_type email;
char msg[80]; 
socklen_t len;

sock_desc=socket(AF_INET,SOCK_DGRAM,0);
if(sock_desc==-1)handle_error("socket");

memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=3001;
server.sin_addr.s_addr=inet_addr("127.0.0.1");
k=bind(sock_desc,(struct sockaddr *)&server,sizeof(server));
if(k==-1)handle_error("bind");

memset(&client,0,sizeof(client));
client.sin_family=AF_INET;
client.sin_port=3001;
client.sin_addr.s_addr=inet_addr("127.0.0.2");
len=sizeof(client);
k=recvfrom(sock_desc,(void *)&email,sizeof(email),0,(struct sockaddr *)&client,&len);
if(k==-1)handle_error("recvfrom");

for(i=0;i<strlen(email.to);i++)
	{
	if(email.to[i]=='@')
		{
		for(j=i+1,k=0;j<strlen(email.to);j++,k++)msg[k]=email.to[j];
		msg[k]='\0';
		break;		
		}
	}
printf("Domain verified\nDOMAIN: %s\n",msg);
printf("-----Message received-----\n");
printf("MAIL TO: %s\nMAIL FROM: %s\nSUBJECT: %s\n",email.to,email.from,email.subject);
printf("BODY:\n%s\n",email.body);

strcpy(msg,"ACK");
k=sendto(sock_desc,(void *)msg,sizeof(msg),0,(struct sockaddr *)&client,sizeof(client));
if(k==-1)handle_error("sendto");

close(sock_desc);
return 0;
}
