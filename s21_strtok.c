#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  char *token = s21_NULL;
  static char *cursor = s21_NULL;
  int start = 1;
  if (!str) {
    start = 0;
    str = cursor;
  }
  if (str && delim) {
    int position;
    if (start) {
      position = s21_strspn(str, delim);
      str += position;
    }
    if (*str) {
      token = str;
      position = s21_strcspn(str, delim);
      str += position;
      if (*str) {
        position = s21_strspn(str, delim);
        *str = '\0';
        str += position;
      }
      cursor = str;
    }
  }
  return token;
}
