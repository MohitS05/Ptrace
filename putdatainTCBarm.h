#ifndef putdatainTCBarm_DOT_H
#define putdatainTCBarm_DOT_H

int putdatainTCBa(struct tcb *tcp, struct pt_regs uregs);
int getargs( struct tcb *tcp,struct pt_regs uregs);

#endif