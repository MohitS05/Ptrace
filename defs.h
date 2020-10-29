//#ifndef STRACE_DEFS_H
//# define STRACE_DEFS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <features.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
/* Open-coding isprint(ch) et al proved more efficient than calling
 * generalized libc interface. We don't *want* to do non-ASCII anyway.
 */
/* #include <ctype.h> */
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "kernel_types.h"
//#include "syscallnet2.h"
#include "list.h"


#ifndef MAX_ARGS
#ifdef LINUX_MIPSO32
#define MAX_ARGS	7
#else
#define MAX_ARGS	6
#endif
#endif

#ifndef SUPPORTED_PERSONALITIES
#define SUPPORTED_PERSONALITIES 1
#endif

//struct pt_regs
struct pt_regs {
     long ebx;
     long ecx;
     long edx;
     long esi;
     long edi;
     long ebp;
     long eax;
     int xds;
     int xes;
     long orig_eax;
     long eip;
     int xcs;
     long eflags;
     long esp;
     int xss;
};

struct tcb {
	int flags;		/* See below for TCB_ values */
	int pid;		/* If 0, this tcb is free */
	int qual_flg;		/* qual_flags[scno] or DEFAULT_QUAL_FLAGS + RAW */
#if SUPPORTED_PERSONALITIES > 1
	unsigned int currpers;	/* Personality at the time of scno update */
#endif
	unsigned long u_error;	/* Error code */
	kernel_ulong_t scno;	/* System call number */
	kernel_ulong_t true_scno;	/* Same, but without subcall decoding and shuffling */
	kernel_ulong_t u_arg[MAX_ARGS];	/* System call arguments */
	kernel_long_t u_rval;	/* Return value */
	int sys_func_rval;	/* Syscall entry parser's return value */
	int curcol;		/* Output column for this process */
	FILE *outf;		/* Output file for this process */
	struct staged_output_data *staged_output_data;

	const char *auxstr;	/* Auxiliary info from syscall (see RVAL_STR) */
	void *_priv_data;	/* Private data for syscall decoding functions */
	void (*_free_priv_data)(void *);	/* Callback for freeing priv_data */
	/*  I HAVE STOPPED THIS
	   const struct_sysent *s_ent; /* sysent[scno] or a stub struct for bad
	   * scno.  Use tcp_sysent() macro for access.
	 */
	// I HAVE STOPPED THIS const struct_sysent *s_prev_ent; /* for "resuming interrupted SYSCALL" msg */
	struct inject_opts *inject_vec[SUPPORTED_PERSONALITIES];
	struct timespec stime;	/* System time usage as of last process wait */
	struct timespec ltime;	/* System time usage as of last syscall entry */
	struct timespec atime;	/* System time right after attach */
	struct timespec etime;	/* Syscall entry time (CLOCK_MONOTONIC) */
	struct timespec delay_expiration_time;	/* When does the delay end */

	/*
	 * The ID of the PID namespace of this process
	 * (inode number of /proc/<pid>/ns/pid)
	 * (0: not initialized)
	 */
	unsigned int pid_ns;

	struct mmap_cache_t *mmap_cache;

	/*
	 * Data that is stored during process wait traversal.
	 * We use indices as the actual data is stored in an array
	 * that is realloc'ed at runtime.
	 */
	size_t wait_data_idx;
	/** Wait data storage for a delayed process. */
	struct tcb_wait_data *delayed_wait_data;
	struct list_item wait_list;


#ifdef HAVE_LINUX_KVM_H
	struct vcpu_info *vcpu_info_list;
#endif

#ifdef ENABLE_STACKTRACE
	void *unwind_ctx;
	struct unwind_queue_t *unwind_queue;
#endif
};
