#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *new_str = s21_NULL;
  if (src) {
    s21_size_t len = s21_strlen(src);
    if (start_index <= len) {
      s21_size_t len1 = str == s21_NULL ? 0 : s21_strlen(str);
      new_str = (char *)calloc(len + len1 + 1, sizeof(char));
      if (start_index && str) {
        new_str = s21_strncpy(new_str, src, start_index);
        new_str[start_index] = '\0';
      }
      if (str) new_str = s21_strcat(new_str, str);
      new_str = s21_strcat(new_str, src + start_index);
    }
  }
  return new_str;
}
