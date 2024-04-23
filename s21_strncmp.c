#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  unsigned char *ptr1 = (unsigned char *)str1, *ptr2 = (unsigned char *)str2;
  int result = 0;
  while (*ptr1 && result == 0 && n--) {
    result = *(ptr1++) - *(ptr2++);
  }
  if (*ptr1 == 0 && n != 0) result = -*ptr2;
  return result;
}
