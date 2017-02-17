#include<stdio.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>

struct msgtype
{
long mtype;
char mtext[100];
};


void rev(char str[])
{
int i;
char temp;
for(i=0;i<strlen(str)/2;i++)
	{
	temp=str[i];
	str[i]=str[strlen(str)-i-1];
	str[strlen(str)-i-1]=temp;
	}
}


main()
{
int msg_id;
struct msgtype message;

msg_id=msgget((key_t)2345,IPC_CREAT|0666);
if(msg_id==-1)
	{
	printf("Error in creating MQ\n");
	exit(1);
	}


while(1)
{
if(msgrcv(msg_id,(void *)&message,100,1,0)==-1)
	{
	printf("Error in receiving message");
	exit(1);
	}
printf("Received message: %s\n",message.mtext);
if(strcmp(message.mtext,"quit")==0)break;

rev(message.mtext);
message.mtype=1;
if(msgsnd(msg_id,(void *)&message,100,0)==-1)
	{
	printf("Error in sending message");
	exit(1);
	}
}
msgctl(msg_id,IPC_RMID,NULL);
return 0;
}
