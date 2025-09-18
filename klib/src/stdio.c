#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = 0;
  for(; *fmt != '\0'; fmt++) {
    if(*fmt != '%') { putch(*fmt); cnt++; continue; }
  }
  fmt++;
  switch(*fmt) {
    case 'd' : {
      int n = va_arg(ap, int);
      if(n < 0) { putch('-'); cnt++; n *= -1;}
      if(n == 0) {putch('0'); cnt++;}
      else {
        char buf[16];
        int i = 0;
        while(n > 0) {buf[i++] = '0' + (n % 10); n /= 10;}
        cnt += i;
        while(i > 0) putch(buf[--i]);
      }
      break;
    }
    case 's' : {
      char *str = va_arg(ap, char*);
      while(*str) {putch(*str++); cnt++;}
      break;
    }
    case 'c' : {
      char c = (char)va_arg(ap, int);
      putch(c);
      cnt++;
      break;
    }
    default : panic("Unknown kind of function printf"); break;
  }
  return 0;
  // panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int cnt = 0;
    for(; *fmt != '\0'; fmt++) {
      if(*fmt != '%') { *out++ = *fmt; cnt++; continue; }
      fmt++;
      switch(*fmt) {
        case 'd': {
          int n = va_arg(ap, int);
          if(n < 0) { *out++ = '-'; cnt++; n = -n;}
          if(n == 0) {*out++ = '0'; cnt++;}
          else {
            char buf[16];int i = 0;
            while(n > 0) {buf[i++] = '0' + (n % 10); n /= 10;}
            cnt += i;
            while(i > 0) {*out++ = buf[--i];}
          }
          break;
        }
        case 's': {
          char *str = va_arg(ap, char*);
          while(*str) {*out++ = *str++; cnt++;}
          break;
        }
        case 'c': {
          char c = (char)va_arg(ap, int);*out++ = c;cnt++;
          break;
        }
        default : panic("Unknown kind of function printf"); break;
      }
    }
    *out = '\0';
    return cnt;
  // panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = vsprintf(out, fmt, ap);
  va_end(ap);
  return cnt;
  // panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int result = vsnprintf(out, n, fmt, ap);
  va_end(ap);
  return result;
  // panic("Not implemented");
}
  
int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  if (n == 0) return 0;
  va_list ap_len;
  va_copy(ap_len, ap);
  int total_len = 0;
  for (const char *p = fmt; *p != '\0'; p++) {
    if (*p != '%') { total_len++; continue; }
    p++;
    switch (*p) {
      case 'd': {
        int num = va_arg(ap_len, int);
        if (num < 0) total_len++;
        if (num == 0) { total_len++; }
        else { while (num > 0) { total_len++; num /= 10; } }
        break;
        }
       case 's': {
        char *str = va_arg(ap_len, char*);
        while (*str != '\0') { total_len++; str++; }
        break;
        }
        case 'c': {
          total_len++;
          break;
        }
        case '%': {
          total_len++;
          break;
        }
        default: { total_len += 2; break; }
      }
    }
    va_end(ap_len);

    if (n > (size_t)total_len) {
        return vsprintf(out, fmt, ap);
    }
    
    char *temp = (char*)malloc(total_len + 1);
    if (temp == NULL) { return -1; }

    int result = vsprintf(temp, fmt, ap);
    strncpy(out, temp, n - 1);
    out[n - 1] = '\0';
    
    free(temp);
    return result;
  // panic("Not implemented");
}

#endif
