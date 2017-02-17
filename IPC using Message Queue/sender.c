#include<stdio.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>

struct msgtype
{
long mtype;
char mtext[100];
};

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
message.mtype=1;
printf("Enter message to reverse:");
gets(message.mtext);
if(msgsnd(msg_id,(void *)&message,100,0)==-1)
	{
	printf("Error in sending message");
	exit(1);
	}
if(strcmp(message.mtext,"quit")==0)break;

if(msgrcv(msg_id,(void *)&message,100,1,0)==-1)
	{
	printf("Error in receiving message");
	exit(1);
	}
printf("Reversed message: %s\n",message.mtext);
}
msgctl(msg_id,IPC_RMID,NULL);
return 0;
}
