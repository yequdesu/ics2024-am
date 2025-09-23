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
    fmt++;
    if(*fmt == '\0') break;
    switch(*fmt) {
      case 'd': {
        int n = va_arg(ap, int);
        if(n < 0) {
          // if(n == (int)0x80000000) {
          //   char *str = "-2147483648";
          //   putstr(str);
          //   cnt += 11;
          //   break;
          // }
          putch('-');
          cnt++;
          n = -n; // may cause overflow?: -2147483648 -> 2147483648(overflow!)
        }
        if(n == 0) { putch('0'); cnt++; }
        else {
          char buf[16];
          int i = 0;
          while(n > 0) {buf[i++] = '0' + (n % 10); n /= 10;}
          cnt += i;
          while(i > 0) putch(buf[--i]);
        }
        break;
      }
      case 's': {
        char *str = va_arg(ap, char*);
        while(*str) {putch(*str++); cnt++;}
        break;
      }
      case 'c': {
        char c = (char)va_arg(ap, int);
        putch(c);
        cnt++;
        break;
      }
      case 'f': {
        double num = va_arg(ap, double);

        char buf[32];
        int i = 0;
        if (num < 0) { buf[i++] = '-'; num = -num; }
        int num_int = (int)num;
        double frac = num - num_int;
        frac += 0.0000005;
        if (num_int == 0) {
            buf[i++] = '0';
        } else {
            char temp[16];
            int j = 0;
            while (num_int > 0) { temp[j++] = '0' + (num_int % 10); num_int /= 10; }
            for (int k = j - 1; k >= 0; k--) { buf[i++] = temp[k]; }
        }
        buf[i++] = '.';
        for (int k = 0; k < 6; k++) {
            frac *= 10;
            int digit = (int)frac;
            buf[i++] = '0' + digit;
            frac -= digit;
        }
        buf[i] = '\0';
        putstr(buf);
        break;
      }
      case 'x': {
          unsigned int num = va_arg(ap, unsigned int);
          char buf[16];
          int i = 0;
          if (num == 0) { putch('0'); break; }
          while (num > 0) {
              int rmd = num % 16;
              if (rmd < 10) {
                  buf[i++] = '0' + rmd;
              } else {
                  buf[i++] = 'a' + (rmd - 10);
              }
              num /= 16;
          }
          
          for (int j = 0; j < i / 2; j++) {
              char temp = buf[j];
              buf[j] = buf[i - j - 1];
              buf[i - j - 1] = temp;
          }
          
          buf[i] = '\0';
          putstr(buf);
          break;
      }

      case '%': {
        putch('%');
        cnt++;
        break;
      }
      default: 
        putch('%');
        putch(*fmt);
        cnt += 2;
        break;
    }
  }
  va_end(ap);
  return cnt;
  // panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
#ifdef __TEST__
  putstr("kilb: stdio: vsprintf\n");
#endif
  int cnt = 0;
  for(; *fmt != '\0'; fmt++) {
    if(*fmt != '%') { *out++ = *fmt; cnt++; continue; }
    fmt++;
    if(*fmt == '\0') break;
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
      case 'f': {
        double num = va_arg(ap, double);

        char buf[32];
        int i = 0;
        if (num < 0) { buf[i++] = '-'; num = -num; }
        int num_int = (int)num;
        double frac = num - num_int;
        frac += 0.0000005;
        if (num_int == 0) {
            buf[i++] = '0';
        } else {
            char temp[16];
            int j = 0;
            while (num_int > 0) { temp[j++] = '0' + (num_int % 10); num_int /= 10; }
            for (int k = j - 1; k >= 0; k--) { buf[i++] = temp[k]; }
        }
        buf[i++] = '.';
        for (int k = 0; k < 6; k++) {
            frac *= 10;
            int digit = (int)frac;
            buf[i++] = '0' + digit;
            frac -= digit;
        }
        buf[i] = '\0';
        char *str = buf;
        while(*str) {*out++ = *str++; cnt++;}
        break;
      }
      case 'x': {
          unsigned int num = va_arg(ap, unsigned int);
          char buf[16];
          int i = 0;
          if (num == 0) { *out++ = '0'; cnt++; break; }
          while (num > 0) {
              int rmd = num % 16;
              if (rmd < 10) {
                  buf[i++] = '0' + rmd;
              } else {
                  buf[i++] = 'a' + (rmd - 10);
              }
              num /= 16;
          }
          
          for (int j = 0; j < i / 2; j++) {
              char temp = buf[j];
              buf[j] = buf[i - j - 1];
              buf[i - j - 1] = temp;
          }
          buf[i] = '\0';
          char *str = buf;
          while(*str) {*out++ = *str++; cnt++;}
          break;
      }
      case 's': {
        char *str = va_arg(ap, char*);
        while(*str) {*out++ = *str++; cnt++;}
        break;
      }
      case 'c': {
        char c = (char)va_arg(ap, int); *out++ = c; cnt++;
        break;
      }
      case '%': {
        *out++ = '%'; cnt++;
        break;
      }
      default: 
        *out++ = '%';
        *out++ = *fmt;
        cnt += 2;
        break;
    }
  }
  *out = '\0';
  return cnt;
  // panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
#ifdef __TEST__
  putstr("kilb: stdio: sprintf\n");
#endif
  va_list ap;
  va_start(ap, fmt);
  int cnt = vsprintf(out, fmt, ap);
  va_end(ap);
  return cnt;
  // panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
#ifdef __TEST__
  putstr("kilb: stdio: printf\n");
#endif
  va_list ap;
  va_start(ap, fmt);
  int result = vsnprintf(out, n, fmt, ap);
  va_end(ap);
  return result;
  // panic("Not implemented");
}
  
int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
#ifdef __TEST__
  putstr("kilb: stdio: vsnprintf\n");
#endif
  if (n == 0) return 0;
  
  size_t out_index = 0;
  for(; *fmt != '\0' && out_index < n - 1; fmt++) {
    if(*fmt != '%') { out[out_index++] = *fmt; continue; }
    
    fmt++;
    if(*fmt == '\0') break;
    
    switch(*fmt) {
      case 'd': {
        int num = va_arg(ap, int);
        char buf[16];
        int i = 0;
        if(num < 0) {
          if(out_index < n - 1) out[out_index++] = '-';
          num = -num;
        }
        if(num == 0) {
          if(out_index < n - 1) out[out_index++] = '0';
        } else {
          while(num > 0) {
            buf[i++] = '0' + (num % 10);
            num /= 10;
          }
          while(i > 0 && out_index < n - 1) {
            out[out_index++] = buf[--i];
          }
        }
        break;
      }
      case 'x': {
        unsigned int num = va_arg(ap, unsigned int);
        char buf[16];
        int i = 0;
        if(num == 0) {
          if(out_index < n - 1) out[out_index++] = '0';
        } else {
          while(num > 0) {
            int rmd = num % 16;
            buf[i++] = (rmd < 10) ? ('0' + rmd) : ('a' + rmd - 10);
            num /= 16;
          }
          while(i > 0 && out_index < n - 1) {
            out[out_index++] = buf[--i];
          }
        }
        break;
      }
      case 'f': {
        double num = va_arg(ap, double);
        char buf[32];
        int i = 0;
        
        if(num < 0) {
          if(out_index < n - 1) out[out_index++] = '-';
          num = -num;
        }
        int num_int = (int)num;
        double frac = num - num_int;
        frac += 0.0000005;
        if(num_int == 0) {
          buf[i++] = '0';
        } else {
          char temp[16];
          int j = 0;
          while(num_int > 0) {
            temp[j++] = '0' + (num_int % 10);
            num_int /= 10;
          }
          for(int k = j - 1; k >= 0; k--) {
            buf[i++] = temp[k];
          }
        }
        buf[i++] = '.';
        for(int k = 0; k < 6; k++) {
          frac *= 10;
          int digit = (int)frac;
          buf[i++] = '0' + digit;
          frac -= digit;
        }
        buf[i] = '\0';
        char *str = buf;
        while(*str && out_index < n - 1) {
          out[out_index++] = *str++;
        }
        break;
      }
      case 's': {
        char *str = va_arg(ap, char*);
        while(*str && out_index < n - 1) {
          out[out_index++] = *str++;
        }
        break;
      }
      case 'c': {
        char c = (char)va_arg(ap, int);
        if(out_index < n - 1) out[out_index++] = c;
        break;
      }
      case '%': {
        if(out_index < n - 1) out[out_index++] = '%';
        break;
      }
      default: {
        if(out_index < n - 1) out[out_index++] = '%';
        if(out_index < n - 1) out[out_index++] = *fmt;
        break;
      }
    }
  }
  out[out_index] = '\0';
  return out_index;
  // panic("Not implemented");
}


#endif
