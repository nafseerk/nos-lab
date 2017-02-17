struct intpair{
int a;
int b;
};

program MATHPROG{
	version MATHVERS{
		int MATHPROC_ADD(intpair)=1;
		int MATHPROC_SUB(intpair)=2;
		int MATHPROC_MULT(intpair)=3;
	}=1;
}=0x20000008;
