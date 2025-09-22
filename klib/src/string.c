#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
#ifdef __TEST__
  printf("kilb: string: strlen\n");
#endif
  size_t cnt = 0;
  while(*s != '\0') { cnt++; }
  return cnt;
  // panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
#ifdef __TEST__
  printf("kilb: string: strcpy\n");
#endif
  char *ptr = dst;
  while ((*ptr++ = *src++) != '\0') {}
  return dst;
  // panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
#ifdef __TEST__
  printf("kilb: string: strncpy\n");
#endif
  char *d = dst;
  while (n > 0 && *src != '\0') { *d++ = *src++; n--; }
  while (n > 0) { *d++ = '\0'; n--; }
  return dst;
  // panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
#ifdef __TEST__
  printf("kilb: string: strcat\n");
#endif
  char *ptr = dst;
  while (*ptr != '\0') { ptr++; }
  while (*src != '\0') { *ptr++ = *src++; }
  *ptr = '\0';
  return dst;
  // panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
#ifdef __TEST__
  printf("kilb: string: strcmp\n");
#endif
  while (*s1 && (*s1 == *s2)) {
    s1++; s2++;
  }
  return *(unsigned char *)s1 - *(unsigned char *)s2;
  // panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
#ifdef __TEST__
  printf("kilb: string: strncmp\n");
#endif
    if (n == 0) return 0;
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    size_t cnt = 0;
    while (cnt < n && *p1 && *p2 && *p1 == *p2) {
        p1++; p2++; cnt++;
    }
    if (cnt == n) { return 0; }
  return *p1 - *p2;
  // panic("Not implemented");
}


void *memset(void *s, int c, size_t n) {
#ifdef __TEST__
  printf("kilb: string: memset\n");
#endif
  unsigned char *ptr = (unsigned char *)s;
  unsigned char value = (unsigned char)c;
  for (size_t i = 0; i < n; i++) {ptr[i] = value;}
  return s;
  // panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
#ifdef __TEST__
  printf("kilb: string: memmove\n");
#endif
  if (n == 0) return dst;
  char *d = (char *)dst;
  const char *s = (const char *)src;
  if (d > s && d < s + n) {
    d += n - 1;
    s += n - 1;
    while (n-- > 0) { *d-- = *s--; }
  } else {
    while (n-- > 0) { *d++ = *s++; }
  }
  return dst;
  // panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
#ifdef __TEST__
  printf("kilb: string: memcpy\n");
#endif
  if (n == 0) return out;
  char *d = (char *)out;
  const char *s = (const char *)in;
  while (n-- > 0) { *d++ = *s++; }
  return out;
  // panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
#ifdef __TEST__
  printf("kilb: string: memcmp\n");
#endif
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;
  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {return p1[i] - p2[i];}
  }
  return 0;
  // panic("Not implemented");
}

#endif
