#include<rpc/rpc.h>
#include"math.h"

main(int argc,char *argv[])
{
CLIENT *c1;
intpair pair;
int *result;
if(argc!=4)
	{
	printf("Usage: prog server num1 num2\n");
	exit(1);
	}
c1=clnt_create(argv[1],MATHPROG,MATHVERS,"udp");
if(c1==NULL)
	{	
	clnt_pcreateerror(argv[1]);
	exit(1);	
	}
pair.a=atoi(argv[2]);
pair.b=atoi(argv[3]);

result=mathproc_add_1(&pair,c1);
if(result==NULL)
	{
	clnt_perror(c1,"add");
	exit(1);
	}
printf("addition result=%d\n",*result);

result=mathproc_sub_1(&pair,c1);
if(result==NULL)
	{
	clnt_perror(c1,"sub");
	exit(1);
	}
printf("subtraction result=%d\n",*result);

result=mathproc_mult_1(&pair,c1);
if(result==NULL)
	{
	clnt_perror(c1,"mult");
	exit(1);
	}
printf("multiplication result=%d\n",*result);

}
