#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

main()
{
char readpipe[]="pipe1";
char writepipe[]="pipe2";
char str[50],temp;
int frd,fwr;
int i,size;

if(access(readpipe,F_OK)==-1)
	if(mkfifo(readpipe,0777)==-1)
		{
		printf("Error in creating read pipe\n");
		exit(1);
		}

if(access(writepipe,F_OK)==-1)
	if(mkfifo(writepipe,0777)==-1)
		{
		printf("Error in creating write pipe\n");
		exit(1);
		}
	
if((frd=open(readpipe,O_RDONLY))==-1)
	{
	printf("Error in opening read pipe\n");
	exit(1);
	}	

if((fwr=open(writepipe,O_WRONLY))==-1)
	{
	printf("Error in opening write pipe\n");
	exit(1);
	}	

read(frd,(void *)str,sizeof(str));
printf("Received string: %s\n",str);

size=strlen(str);
for(i=0;i<size/2;i++)
	{
	temp=str[i];
	str[i]=str[size-i-1];
	str[size-i-1]=temp;
	}

write(fwr,(void *)str,sizeof(str));
close(fwr);
close(frd);
return 0;
}
