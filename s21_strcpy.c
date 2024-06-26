#include "s21_string.h"

char *s21_strcpy(char *dest, const char *src) {
  char *ptr = dest;
  if (dest && src) {
    while (*src) {
      *dest = *src;
      dest++;
      src++;
    }
    *dest = '\0';
  }
  return ptr;
}
