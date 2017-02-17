#include<stdio.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>

main()
{
pid_t id;
id=fork();
if(id==0)   //child
	{
	int seg_id;
	int *addr;
	int c,d;
	seg_id=shmget((key_t)2345,sizeof(int),IPC_CREAT|0666);
	addr=(int *)shmat(seg_id,NULL,0);
	printf("Enter c & d:");
	scanf("%d%d",&c,&d);
	*addr= c*d;
	shmdt(addr);
	shmctl(seg_id,IPC_RMID,NULL);
	return 0;
	}
else if(id>0)    //parent
	{
	int seg_id;
	int *addr;
	int a,b;
	seg_id=shmget((key_t)2345,sizeof(int),IPC_CREAT|0666);
	addr=(int *)shmat(seg_id,NULL,0);
	wait(NULL);
	printf("Enter a & b:");
	scanf("%d%d",&a,&b);
	*addr=  *addr + a*b;
	printf("Result=%d\n",*addr);
	shmdt(addr);
	shmctl(seg_id,IPC_RMID,NULL);
	return 0;
	}
}
