#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static unsigned long int next = 1;

int rand(void) {
#ifdef __TEST__
  putstr("kilb: stdlib: rand\n");
#endif
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
#ifdef __TEST__
  putstr("kilb: stdlib: srand\n");
#endif
  next = seed;
}

int abs(int x) {
#ifdef __TEST__
  putstr("kilb: stdlib: abs\n");
#endif
  return (x < 0 ? -x : x);
}

int atoi(const char* nptr) {
#ifdef __TEST__
  putstr("kilb: stdlib: atoi\n");
#endif
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}

char *brk = NULL;

void *malloc(size_t size) {
#ifdef __TEST__
  putstr("kilb: stdlib: malloc\n");
#endif
  // On native, malloc() will be called during initializaion of C runtime.
  // Therefore do not call panic() here, else it will yield a dead recursion:
  //   panic() -> putchar() -> (glibc) -> malloc() -> panic()
  if(brk == NULL) brk = (void *)ROUNDUP(heap.start, 8);

  size = (size_t)ROUNDUP(size, 8);
  char *old = brk;
  brk += size;

  assert((uintptr_t)heap.start <= (uintptr_t)brk && (uintptr_t)brk < (uintptr_t)heap.end);
  for (uint64_t *p = (uint64_t *)old; p != (uint64_t *)brk; p ++) {
    *p = 0;
  }

  return old;
  // panic("Not implemented");
  return NULL;
}

void free(void *ptr) {
}

#endif
