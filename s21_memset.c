#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *str_ptr = (unsigned char *)str;
  unsigned char chr = (unsigned char)c;
  while (n--) {
    *str_ptr++ = chr;
  }
  return str;
}
