#include "s21_string.h"

void *s21_to_lower(const char *str) {
  char *str_lower = s21_NULL;
  if (str) {
    s21_size_t size = s21_strlen(str) + 1;
    str_lower = (char *)calloc(size, sizeof(char));
    str_lower = s21_strcpy(str_lower, str);
    int i = 0;
    while (str_lower[i]) {
      if (str_lower[i] >= 'A' && str_lower[i] <= 'Z') {
        str_lower[i] = str_lower[i] + 32;
      }
      i++;
    }
  }
  return str_lower;
}
