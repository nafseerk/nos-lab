#include<stdio.h>
#include<string.h>
#include<sys/statvfs.h>

main(int argc,char *argv[])
{
char Path[128];
struct statvfs Data;
int i;

if(argc<2)
	{
	printf("Usage: <executable> device0 device1 ... deviceN\n");
	return 0;
	}
else
	{
	for(i=1;i<argc;i++)
		{
		strcpy(Path,argv[i]);
		if(statvfs(Path,&Data)<0)printf("Failed to stat: %s\n",Path);
		else
			{
			printf("Device: %s\n",Path);
			printf("\tBlock size: %lu\n",Data.f_bsize);
			printf("\tTotal no of blocks: %u\n",(unsigned)Data.f_blocks);
			printf("\tFree blocks: %u\n",(unsigned)Data.f_bfree);
			printf("\tMax filename length: %lu\n",Data.f_namemax);
			}
		}
	}
}

