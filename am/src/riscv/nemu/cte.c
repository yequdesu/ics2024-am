#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

/*
struct Context {
  uintptr_t gpr[32];
  uintptr_t mcause, mstatus, mepc;
  void *pdir;
  uintptr_t np;
};
*/

void context_display(Context *ctx) {
  if (ctx == NULL) {
    printf("Context: NULL\n");
    return;
  }
  
  printf("General Registers:\n");
  const char *reg_names[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
                            "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
                            "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
                            "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
  
  for (int i = 0; i < 32; i++) {
    if (i % 4 == 0) printf("  ");
    printf("%s: 0x%x", reg_names[i], ctx->gpr[i]);
    printf((i % 4 == 3) ? "\n" : " | ");
  }

  printf("\nCSR Registers:\n");
  printf("  mcause: 0x%x\n", ctx->mcause);
  printf("  mstatus: 0x%x\n", ctx->mstatus);
  printf("  mepc: 0x%x\n", ctx->mepc);

  printf("Other:\n");
  printf("  pdir: 0x%x\n", ctx->pdir);
  printf("  np: 0x%x\n", ctx->np);
}


Context* __am_irq_handle(Context *c) {
  // context_display(c);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 0xffffffff: ev.event = EVENT_YIELD; break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }
  return c;
}


extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  // which is the address of __am_asm_trap in trap.S
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
