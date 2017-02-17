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
struct sockaddr_in server;
int sock_desc;
int k,i,j;
char msg[50],command[10];
int fd,len,c,l;
char length[10],fil[100];
char filename[20];
struct stat obj;

sock_desc=socket(AF_INET,SOCK_STREAM,0);
if(sock_desc==-1)handle_error("socket");

memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=3001;
server.sin_addr.s_addr=inet_addr("127.0.0.1");
k=connect(sock_desc,(struct sockaddr *)&server,sizeof(server));
if(k==-1)handle_error("connect");

printf("Established FTP connection with server\n\n");

while(1)
{
printf(">>>");
gets(msg);
for(i=0;msg[i]!=' '&&msg[i]!='\n'&&msg[i]!='\0';i++)command[i]=msg[i];
command[i]='\0';
if(strcmp(command,"list")==0)
	{
	k=send(sock_desc,(void *)msg,sizeof(msg),0);
	if(k==-1)handle_error("send");
	fd=open("list2.txt",O_RDWR|O_CREAT,0700);
	if(fd==-1)handle_error("open");
	k=recv(sock_desc,(void *)length,sizeof(length),0);
	if(k==-1)handle_error("recv");
	len=atoi(length);
	for(c=0;c<len;)
		{
		l=read(sock_desc,fil,sizeof(fil));
		if(l==0)break;
		write(fd,fil,l);
		c+=l;		
		}
	close(fd);
	system("cat list2.txt");
	printf("\n");
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
	strcat(filename,"(download)");
	fd=open(filename,O_RDWR|O_CREAT,0700);
	if(fd==-1)handle_error("open");
	k=send(sock_desc,(void *)msg,sizeof(msg),0);
	if(k==-1)handle_error("send");
	k=recv(sock_desc,(void *)length,sizeof(length),0);
	if(k==-1)handle_error("recv");
	len=atoi(length);
	for(c=0;c<len;)
		{
		l=read(sock_desc,fil,sizeof(fil));
		if(l==0)break;
		write(fd,fil,sizeof(fil));
		c+=l;		
		}
	close(fd);
	printf("Download Complete\n");		
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
	fd=open(filename,O_RDONLY);
	if(fd==-1)handle_error("open");
	k=send(sock_desc,(void *)msg,sizeof(msg),0);
	if(k==-1)handle_error("send");
	stat(filename,&obj);
	sprintf(length,"%d",(int)obj.st_size);
	k=send(sock_desc,(void *)length,sizeof(length),0);
	if(k==-1)handle_error("send");
	k=sendfile(sock_desc,fd,NULL,obj.st_size);
	if(k==-1)handle_error("sendfile");
	close(fd);	
	printf("Upload complete\n");	
	}
else if(strcmp(command,"clear")==0)
	{
	system("clear");
	}
else if(strcmp(command,"exit")==0)
	{
	printf("FTP connection terminated\n");
	break;
	}
else
	{
	printf("Unknown command: %s\n",msg);
	}
}

close(sock_desc);
return 0;

}
