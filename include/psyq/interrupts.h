#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#define MAX_INTERRUPT_CALLBACKS 8

#define JB_PC 0
#define JB_SP 1
#define JB_FP 2
#define JB_S0 3
#define JB_S1 4
#define JB_S2 5
#define JB_S3 6
#define JB_S4 7
#define JB_S5 8
#define JB_S6 9
#define JB_S7 10
#define JB_GP 11
#define JB_SIZE 12

typedef int jmp_buf[JB_SIZE];

typedef void (*VoidCallback_t)(void);
typedef void (*SetVsyncIntrCallback_t)(unsigned int, void(*)(void));

typedef struct {
    const char* version;
    void*(*setDMAIntrCallback)(unsigned int index, VoidCallback_t callback);
    void(*setIntrHandler)(unsigned int irqLine, VoidCallback_t callback);
    void*(*intrResetHandler)(void);
    void*(*intrStopHandler)(void);
    void*(*setVsyncIntrCallback)(unsigned int index, VoidCallback_t callback);
    void*(*intrRestartHandler)(void);
} InterruptControl_t;

typedef struct {
    u_short interruptsInitialized;
    u_short inInterrupt;
    VoidCallback_t handlers[11];
    u_short enabledInterruptsMask;
    u_short savedMask;
    int savedPcr;
    jmp_buf buf;
    int stack[1024];
} InterruptEnvironment_t;

extern InterruptEnvironment_t g_InterruptEnvironment;
extern InterruptControl_t g_InterruptControl;
extern InterruptControl_t* g_pInterruptControl;

extern VoidCallback_t g_VsyncInterruptCallbacks[MAX_INTERRUPT_CALLBACKS];
extern VoidCallback_t g_DmaInterruptCallbacks[MAX_INTERRUPT_CALLBACKS];

extern volatile int g_VsyncInterruptCount;
extern volatile int g_VsyncPrevInterruptCount;
extern volatile int g_HsyncInterruptCount;

extern int D_8005893C;

extern volatile u_short* g_pI_STAT;
extern volatile u_short* g_pI_MASK;
extern volatile int* g_pDMA_DPCR;
extern volatile int* g_pGPUSTAT;
extern volatile int* g_pTMR_HRETRACE_VAL;
extern volatile u_long* g_pTMR_HRETRACE_MODE;
extern volatile u_long* g_pDMA_DICR;

extern long g_VideoMode;


#endif