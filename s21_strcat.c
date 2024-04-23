#include "s21_string.h"

char *s21_strcat(char *dest, const char *src) {
  char *ptr = dest + s21_strlen(dest);
  while (*src) {
    *ptr++ = *src++;
  }
  *ptr = '\0';
  return dest;
}
