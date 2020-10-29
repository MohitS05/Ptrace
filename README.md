# Ptrace

Making a lightweight debugger, kind of like strace but for Embedded systems. Implemented using Ptrace. For arm systems couldn't find the library with regs, so made a structure myself (defined in defs.h)




To compile for Arm:
arm-marvell-linux-gnueabi-gcc -o Ctracer Maintracer.c syscallnet2.h defs.h kernel_types.h macros.h list.h syscallnetarm.h putdatainTCBarm.c putdatainTCBarm.h 

To compile for x86:
gcc -o trac Maintracer.c syscallnet2.h defs.h kernel_types.h macros.h list.h putdatainTCB.h putdatainTCB.c
