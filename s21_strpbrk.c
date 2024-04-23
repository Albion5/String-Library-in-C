#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *ptr1 = (char *)str1;
  while (*ptr1 && s21_strchr(str2, *ptr1) == s21_NULL) {
    ptr1++;
  }
  if (*ptr1 == 0) ptr1 = s21_NULL;
  return ptr1;
}
