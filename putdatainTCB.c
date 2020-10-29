
#include <sys/user.h>
#include <stdio.h>
#include <time.h>
#include "defs.h"

int getargs(struct user_regs_struct uregs, struct tcb *tcp);
int putdatainTCB(struct tcb *tcp,struct user_regs_struct uregs)
{
	tcp->scno = uregs.orig_rax;
	tcp->u_rval = uregs.rax;
#ifdef X86
	timespec_get(&(tcp->atime), TIME_UTC);
#endif
#ifdef arm
	timespec_get(&(tcp->atime), 5);
#endif
	/*
	clock_t begin;
	tcp->atime = begin;
	*/
	tcp->u_error = getargs(uregs, tcp);

	if (tcp->u_error) {
		fprintf(stderr, "Error occured at getargs() error = %ld \n",tcp->u_error);
	}
}


/* Copy values in all registers to tcp */
int getargs(struct user_regs_struct uregs, struct tcb *tcp)
{
	//printf("Getting arguments \n");
	tcp->u_arg[0] = uregs.rdi;
	//printf(" val 0 = %d ", tcp->u_arg[0])
	tcp->u_arg[1] = uregs.rsi;
	tcp->u_arg[2] = uregs.rdx;
	tcp->u_arg[3] = uregs.r10;
	tcp->u_arg[4] = uregs.r8;
	tcp->u_arg[5] = uregs.r9;

	/* Debug code to check value after inserting
	for (i = 0; i < 4; ++i)
		printf("%s%#" PRI_klx, i ? ", " : "", tcp->u_arg[i]);
    */ 
	return 0;
}
