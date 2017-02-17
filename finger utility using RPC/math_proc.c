#include<rpc/rpc.h>
#include"math.h"

int *mathproc_add_1_svc(intpair * pair,struct svc_req *rqstp)
{
static int result;
result=pair->a + pair->b;
return(&result);
}

int *mathproc_sub_1_svc(intpair * pair,struct svc_req *rqstp)
{
static int result;
result=pair->a - pair->b;
return(&result);
}

int *mathproc_mult_1_svc(intpair * pair,struct svc_req *rqstp)
{
static int result;
result=pair->a * pair->b;
return(&result);
}
