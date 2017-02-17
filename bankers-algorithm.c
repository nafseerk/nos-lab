#include<stdio.h>
#include<stdlib.h>

int m,n;
int available[20],allocated[20][20],maximum[20][20],need[20][20];

int compare(int arr1[],int arr2[],int size)
{
int i;

for(i=0;i<size;i++)
	{
	if(arr1[i]>arr2[i])return 1;
	}
return 0;
}

main()
{
int i,j,work[20],finish[20];
printf("Enter m &n:");
scanf("%d%d",&m,&n);
printf("Enter Available Vector:");
for(i=0;i<m;i++)scanf("%d",&available[i]);
printf("Enter Maximum matrix:");
for(i=0;i<n;i++)
	for(j=0;j<m;j++)
		scanf("%d",&maximum[i][j]);

printf("Enter Allocated matrix:");
for(i=0;i<n;i++)
	for(j=0;j<m;j++)
		scanf("%d",&allocated[i][j]);

printf("Need matrix:");
for(i=0;i<n;i++)
	{
	for(j=0;j<m;j++)
		{
		need[i][j]=maximum[i][j]-allocated[i][j];
		printf("%d ",need[i][j]);
		}
	printf("\n");
	}

for(i=0;i<m;i++)work[i]=available[i];
for(i=0;i<n;i++)finish[i]=0;

for(i=0;i<n;)
	{
	if(finish[i]==0 && compare(need[i],work,m)==0)
		{
		for(j=0;j<m;j++)
			{
			work[j]=work[j]+allocated[i][j];
			allocated[i][j]=0;
			}
		finish[i]=1;
		i=0;
		}	
	else i++;
	}

for(i=0;i<n;i++)
	{
	if(finish[i]==0)
		{
		printf("Unsafe\n");
		exit(1);
		}
	}
printf("Safe state\n");
}

