#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  int i = s21_strlen(str);
  unsigned char conv_c = c;
  char *result = s21_NULL;
  while (i > 0 && str[i] != conv_c) {
    --i;
  }
  if (i >= 0 && conv_c == str[i]) {
    result = (char *)str + i;
  }
  return result;
}
