process_opt_p_list(char *opt)
{
	while (*opt) {
		/*
		 * We accept -p PID,PID; -p "`pidof PROG`"; -p "`pgrep PROG`".
		 * pidof uses space as delim, pgrep uses newline. :(
		 */
		int pid;
		char *delim = opt + strcspn(opt, "\n\t ,");
		char c = *delim;

		*delim = '\0';
		pid = string_to_uint(opt);
		if (pid <= 0) {
			error_msg_and_die("Invalid process id: '%s'", opt);
		}
		if (pid == strace_tracer_pid) {
			error_msg_and_die("I'm sorry, I can't let you do that, Dave.");
		}
		*delim = c;
		alloctcb(pid);
		if (c == '\0')
			break;
		opt = delim + 1;
	}
}
static struct tcb *
alloctcb(int pid)
{
	unsigned int i;
	struct tcb *tcp;

	if (nprocs == tcbtabsize)
		expand_tcbtab();

	for (i = 0; i < tcbtabsize; i++) {
		tcp = tcbtab[i];
		if (!tcp->pid) {
			memset(tcp, 0, sizeof(*tcp));
			list_init(&tcp->wait_list);
			tcp->pid = pid;
#if SUPPORTED_PERSONALITIES > 1
			tcp->currpers = current_personality;
#endif
			nprocs++;
			debug_msg("new tcb for pid %d, active tcbs:%d",
				  tcp->pid, nprocs);
			return tcp;
		}
	}
	error_msg_and_die("bug in alloctcb");
}
