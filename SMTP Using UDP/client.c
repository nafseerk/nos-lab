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
int k;
struct email_type email;
char msg[80]; 
socklen_t len;

sock_desc=socket(AF_INET,SOCK_DGRAM,0);
if(sock_desc==-1)handle_error("socket");

memset(&client,0,sizeof(client));
client.sin_family=AF_INET;
client.sin_port=3001;
client.sin_addr.s_addr=inet_addr("127.0.0.2");
k=bind(sock_desc,(struct sockaddr *)&client,sizeof(client));
if(k==-1)handle_error("bind");

printf("\nMAIL TO:");
gets(email.to);
printf("\nMAIL FROM:");
gets(email.from);
printf("\nSUBJECT:");
gets(email.subject);
printf("\nBODY:\n");
strcpy(email.body,"");
strcpy(msg,"");
while(strcmp(msg,".")!=0){ gets(msg); strcat(email.body,msg); strcat(email.body,"\n"); }


memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=3001;
server.sin_addr.s_addr=inet_addr("127.0.0.1");
k=sendto(sock_desc,(void *)&email,sizeof(email),0,(struct sockaddr *)&server,sizeof(server));
if(k==-1)handle_error("sendto");

len=sizeof(server);
k=recvfrom(sock_desc,(void *)msg,sizeof(msg),0,(struct sockaddr *)&server,&len);
if(k==-1)handle_error("recvfrom");
if(strcmp(msg,"ACK")==0)printf("Message delivered succesfully to %s\n",email.to);

close(sock_desc);
return 0;
}
