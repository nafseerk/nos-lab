#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<errno.h>
#include<fcntl.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE);}while(0)

main()
{
int sock_desc,temp_sock_desc;
struct sockaddr_in server,client;
int k,i,j;
socklen_t len;
char msg[50],command[10];
struct stat obj;
int fd;
char length[10],fil[100];
char filename[20];
int c,l;

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

len=sizeof(client);
memset(&client,0,sizeof(client));
temp_sock_desc=accept(sock_desc,(struct sockaddr *)&client,&len);
if(temp_sock_desc==-1)handle_error("accept");

printf("Esatblished connection with FTP client\n\n");

while(1)
{
k=recv(temp_sock_desc,(void *)msg,sizeof(msg),0);
if(k==-1)handle_error("recv");
for(i=0;msg[i]!=' '&&msg[i]!='\n'&&msg[i]!='\0';i++)command[i]=msg[i];
command[i]='\0';
if(strcmp(command,"list")==0)
	{
	system("ls -al>list.txt");
	fd=open("list.txt",O_RDONLY);
	stat("list.txt",&obj);
	sprintf(length,"%d",(int)obj.st_size);
	k=send(temp_sock_desc,(void *)length,sizeof(length),0);
	if(k==-1)handle_error("send");
	k=sendfile(temp_sock_desc,fd,NULL,obj.st_size);
	if(k==-1)handle_error("sendfile");
	close(fd);	
	}
else if(strcmp(command,"load")==0)
	{
	for(i=0;i<strlen(msg);i++)
		{
		if(msg[i]==' ')
			{
			for(j=i+1,k=0;j<strlen(msg);j++,k++)filename[k]=msg[j];
			filename[k]='\0';
			break;				
			}
		}
	fd=open(filename,O_RDONLY);
	if(fd==-1)handle_error("open");
	stat(filename,&obj);
	sprintf(length,"%d",(int)obj.st_size);
	k=send(temp_sock_desc,(void *)length,sizeof(length),0);
	if(k==-1)handle_error("send");
	k=sendfile(temp_sock_desc,fd,NULL,obj.st_size);
	if(k==-1)handle_error("sendfile");
	close(fd);	
	}
else if(strcmp(command,"store")==0)
	{
	for(i=0;i<strlen(msg);i++)
		{
		if(msg[i]==' ')
			{
			for(j=i+1,k=0;j<strlen(msg);j++,k++)filename[k]=msg[j];
			filename[k]='\0';
			break;				
			}
		}
	strcat(filename,"(upload)");
	fd=open(filename,O_RDWR|O_CREAT,0700);
	if(fd==-1)handle_error("open");
	k=recv(temp_sock_desc,(void *)length,sizeof(length),0);
	if(k==-1)handle_error("recv");
	len=atoi(length);
	for(c=0;c<len;)
		{
		l=read(temp_sock_desc,fil,sizeof(fil));
		if(l==0)break;
		write(fd,fil,sizeof(fil));
		c+=l;		
		}
	close(fd);
	}
else if(strcmp(command,"exit")==0)
	{
	printf("FTP connection terminated\n");
	break;
	}

}

close(temp_sock_desc);
close(sock_desc);
return 0;
}
