#include "s21_string.h"

void *s21_to_upper(const char *str) {
  char *str_upper = s21_NULL;
  if (str) {
    s21_size_t size = s21_strlen(str) + 1;
    str_upper = (char *)calloc(size, sizeof(char));
    str_upper = s21_strcpy(str_upper, str);
    int i = 0;
    while (str_upper[i]) {
      if (str_upper[i] >= 'a' && str_upper[i] <= 'z') {
        str_upper[i] = str_upper[i] - 32;
      }
      i++;
    }
  }
  return str_upper;
}
