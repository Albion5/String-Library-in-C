#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *ptr = dest;
  if (dest && src) {
    s21_size_t i = 0;
    while (*src && i < n) {
      *dest = *src;
      dest++;
      src++;
      i++;
    }
    while (i < n) {
      *dest = '\0';
      dest++;
      i++;
    }
  }
  return ptr;
}
