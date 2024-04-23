#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  int i = 0;
  unsigned char conv_c = c;
  char *result = s21_NULL;
  while (str[i] && str[i] != conv_c) {
    ++i;
  }
  if (conv_c == str[i]) {
    result = (char *)str + i;
  }
  return result;
}
