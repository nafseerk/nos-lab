#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

main()
{
char writepipe[]="pipe1";
char readpipe[]="pipe2";
char str[50],rev[50];
int frd,fwr;
if(access(writepipe,F_OK)==-1)
	if(mkfifo(writepipe,0777)==-1)
		{
		printf("Error in creating write pipe\n");
		exit(1);
		}
	
if(access(readpipe,F_OK)==-1)
	if(mkfifo(readpipe,0777)==-1)
		{
		printf("Error in creating read pipe\n");
		exit(1);
		}

if((fwr=open(writepipe,O_WRONLY))==-1)
	{
	printf("Error in opening write pipe\n");
	exit(1);
	}	

if((frd=open(readpipe,O_RDONLY))==-1)
	{
	printf("Error in opening read pipe\n");
	exit(1);
	}	
printf("Enter string to send:");
gets(str);
write(fwr,(void *)str,sizeof(str));
read(frd,(void *)rev,sizeof(str));
printf("Reversed string: %s\n",rev);
close(fwr);
close(frd);
return 0;
}
