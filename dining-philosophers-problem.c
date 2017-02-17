#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


pthread_t phil[10];
pthread_mutex_t chopstick[10];
int n;

void* func(int i)
{
printf("Philosopher %d is thinking\n",i+1);
pthread_mutex_lock(&chopstick[i]);
pthread_mutex_lock(&chopstick[i+1]);
printf("Philosopher %d is eating\n",i+1);
sleep(2);
pthread_mutex_unlock(&chopstick[i]);
pthread_mutex_unlock(&chopstick[i+1]);
printf("Philosopher %d finished eating\n",i+1);
}


main()
{
void * msg;
int i,k;

printf("Enter no of philosophers:");
scanf("%d",&n);

for(i=0;i<n;i++)
{
k=pthread_mutex_init(&chopstick[i],NULL);
if(k==-1)
	{
	printf("Error in initialising mutex\n");
	exit(1);	
	}
}

for(i=0;i<n;i++)
{
k=pthread_create(&phil[i],NULL,(void *)func,(void *)i);
if(k==-1)
	{
	printf("Error in creating thread\n");
	exit(1);	
	}
}

for(i=0;i<n;i++)
{
k=pthread_join(phil[i],&msg);
if(k==-1)
	{
	printf("Error in waiting for threads\n");
	exit(1);	
	}
}

for(i=0;i<n;i++)
{
k=pthread_mutex_destroy(&chopstick[i]);
if(k==-1)
	{
	printf("Error in destroying mutex\n");
	exit(1);	
	}
}


}

