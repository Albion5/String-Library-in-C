#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *ptr = (unsigned char *)str, *result = s21_NULL;
  while (result == s21_NULL && n--) {
    if (*ptr == (unsigned char)c) result = ptr;
    ptr++;
  }
  return (void *)result;
}
