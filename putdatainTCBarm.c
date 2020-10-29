
#include <sys/user.h>
#include <stdio.h>
#include <sys/ptrace.h>
//#include <ptrace.h>
#include <time.h>
#include "defs.h"

int getargs(struct pt_regs uregs, struct tcb *tcp);
int putdatainTCBa(struct tcb *tcp,struct pt_regs uregs)
{
	tcp->scno = uregs.orig_eax;
	tcp->u_rval = uregs.eax;
	//tcp->
	timespec_get(&(tcp->atime), 5);

	tcp->u_error = getargs(uregs, tcp);

	if (tcp->u_error) {
		fprintf(stderr, "Error occured at getargs() error = %ld \n",tcp->u_error);
	}
}


/* Copy values in all registers to tcp */
int getargs(struct pt_regs uregs, struct tcb *tcp)
{
	//printf("Getting arguments \n");


	tcp->u_arg[0] = uregs.edi;
	//printf(" val 0 = %d ", tcp->u_arg[0])
	tcp->u_arg[1] = uregs.esi;
	tcp->u_arg[2] = uregs.edx;
	tcp->u_arg[3] = uregs.ebx;
	tcp->u_arg[4] = uregs.ebp;
	tcp->u_arg[5] = uregs.ecx;

//ebx, ecx, edx, esi, edi, ebp
	/* Debug code to check value after inserting
	for (i = 0; i < 4; ++i)
		printf("%s%#" PRI_klx, i ? ", " : "", tcp->u_arg[i]);
    */ 
	return 0;
}
