#include <sys/ptrace.h>
#include <signal.h>
//#include <ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <stdbool.h>
#include <stdio.h>
#include "defs.h"
#define arm 1	


#ifdef X86
	#include "syscallnet2.h"
	#include <sys/reg.h>
	#include "putdatainTCB.h"
	//int getargs(struct user_regs_struct uregs, struct tcb *tcp);
#endif

#ifdef arm
	//#include <user/reg.h>
	#include "syscallnetarm.h"
	#include "putdatainTCBarm.h"

	//int getargs(struct pt_regs uregs;, struct tcb *tcp);
#endif



int err;

int printsyscall(int);


int printargs(struct tcb *tcp, int, int);
int printargs_d(struct tcb *tcp, int n, int syscall_num);

static struct tcb *alloctcb(int pid)
{
	//printf("allocating tcb \n");
	unsigned int i;
	//printf("allocating tcb5 \n");
	struct tcb *tcp;
	tcp = (struct tcb *) calloc(1, sizeof(struct tcb));
	//printf("allocating tcb4 \n");
	//printf("%d\n",tcp->pid);
	if (!tcp->pid) {

		memset(tcp, 0, sizeof(*tcp));

		list_init(&tcp->wait_list);
		tcp->pid = pid;
		printf("NEW TCB FOR PID %d \n", tcp->pid);
		//debug_msg("new tcb for pid %d, active tcbs:%d", tcp->pid);
		return tcp;
	}
	fprintf(stderr, "Error occured no if at alloctcb\n");
	//error_msg("bug in alloctcb");
}

/*typedef struct sysent {
	unsigned nargs;
	int	sys_flags;
	int	sen;
	int	(*sys_func)();
	const char *sys_name;
} struct_sysent;


static const struct_sysent syscallent[] = {
#include "sycallnet.h"
};*/







int main(int argc, char **argv)
{
	int status = 0, pid;	// addr; 
	pid = strtol(argv[1], NULL, 10);
	unsigned long long int addr, syscall_num = 5, syscall_retval =10, rip;
	#ifdef X86
		struct user_regs_struct uregs;
	#endif

	#ifdef arm
		struct pt_regs uregs;
	#endif

	//scanf("%d", &pid);
	//printf("%d searching \n",pid);
	//printf("Strarting for \n");
	ptrace(PTRACE_ATTACH, pid, 0, 0);
	wait(&status);
	bool flag = true;
	/*ptrace(PTRACE_GETREGS, pid, 0, &uregs);
	   syscall_num = uregs.orig_rax;
	   syscall_retval=uregs.rax;
	   printf("rip: %x\n", uregs.rip);
	   ptrace(PTRACE_GETREGS, pid, 0, &uregs);
	   printf("syscall_num = %lld\n",syscall_num);
	   printf("Syscall Retun val = %lld\n",syscall_retval);
	   //uregs.rip = addr;
	   // ptrace(PTRACE_SETREGS, pid, 0, &uregs);
	 */
	ptrace(PTRACE_GETREGS, pid, 0, &uregs);
	//syscall_num = uregs.orig_rax;
	syscall_num = uregs.orig_eax;
	//syscall_retval = uregs.rax;
	syscall_retval = uregs.eax;
	//printf("Instruction Pointer (rip) : %llx\n", uregs.rip);
	printf("Instruction Pointer (rip) : %llx\n", uregs.eip);
	ptrace(PTRACE_GETREGS, pid, 0, &uregs);
	printf("syscall_num = %lld\n", syscall_num);
	/* Not really of much use right  now
	   printf("Syscall Retun val = %lld\n",syscall_retval);
	 */
	err = printsyscall(syscall_num);
	if (err) {
		fprintf(stderr,
			"Error occured at printsyscall() error = %d \n",
			err);
	}
	ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	// printf("got here2\n");

	/* Starting TCB Tracing control Block */
	struct tcb *tcp;
	//printf("got here3\n");

	tcp = alloctcb(pid);
	//tcp = (struct tcb*)calloc(sizeof(struct*tcb));
	if (tcp == NULL) {
		fprintf(stderr, "Memory not allocated to TCB\n");
	}
	/*  Shifted this functionality to putdatainTCB()
	   err = getargs(uregs, tcp);

	   if(err){
	   fprintf(stderr, "Error occured at getargs() error = %d \n",err );
	   }
	 */

	//printf("got here\n");
	putdatainTCBa(tcp,uregs);
	printargs(tcp, syscallent[syscall_num].nargs, syscall_num);
	printargs_d(tcp, syscallent[syscall_num].nargs, syscall_num);
	//printf("\nWent to print args\n");
	ptrace(PTRACE_CONT, pid, 0, 0);
	ptrace(PTRACE_CONT, pid, 0, 0);
	//printsyscall(syscall_num);
	wait(&status);
	printf("Exiting, snoop over\n");
	if (WIFEXITED(status))
		printf("child over\n");
	/* Cleanup */
	printf("Detaching and Freeing up TCB (Tracer Control Block)\n");
	ptrace(PTRACE_DETACH, pid, NULL, NULL);
	free(tcp);

	//usleep(10);
	return 0;

}




/* 
structure for reference
typedef struct sysent {
	unsigned nargs;
	int	sys_flags;
	int	sen;
	int	(*sys_func)();
	const char *sys_name;
}struct_sysent;

*/

int printsyscall(int syscall_num)
{
	if (syscall_num == 0) {
		fprintf(stderr, "Syscall 0, error in read\n");
		return 1;
	}
	printf("Syscall name = %s \t", syscallent[syscall_num].sys_name);
	printf("Number of args : %u \n", syscallent[syscall_num].nargs);
	printf("Other two vals are SEN = %d and sys_flags = %d \n",
	       syscallent[syscall_num].sen,
	       syscallent[syscall_num].sys_flags);
	return 0;

}



int printargs(struct tcb *tcp, int n, int syscall_num)
{
	//printf("Printing arguments \t amount = %d \n", n);
	int size = n;
	int i;
	printf("%s ( ", syscallent[syscall_num].sys_name);
	for (i = 0; i < size; ++i)
		printf("%s%#" PRI_klx, i ? ", " : "", tcp->u_arg[i]);
	printf(" ) \n\t arguments printed \n");
	return 0;
}

int printargs_d(struct tcb *tcp, int n, int syscall_num)
{
	//printf("Printing arguments from _d \t amount = %d \n", n);
	const int size = n;
	int i;
	printf("%s ( ", syscallent[syscall_num].sys_name);
	for (i = 0; i < size; ++i)
		printf("%s%u", i ? ", " : "",
		       (unsigned int) tcp->u_arg[i]);
	printf(")\n\t arguments printed as unsigned int \n");

	return 0;
}
