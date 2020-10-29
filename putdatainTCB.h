#ifndef putdatainTCB_DOT_H
#define putdatainTCB_DOT_H

int putdatainTCB(struct tcb *tcp, struct user_regs_struct uregs);
int getargs(struct user_regs_struct uregs, struct tcb *tcp);

#endif