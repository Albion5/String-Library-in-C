#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *result = s21_NULL;
  if (haystack != s21_NULL && needle != s21_NULL) {
    int h_len = s21_strlen(haystack);
    int n_len = s21_strlen(needle);
    int end = h_len - n_len + 1;
    int i = 0;
    int found = 0;
    while (i < end && !found) {
      found = 1;
      int j = 0;
      while (j < n_len && found) {
        if (needle[j] != haystack[i + j]) {
          found = 0;
        }
        j++;
      }
      if (!found) {
        i++;
      }
    }
    if (found) {
      result = (char *)haystack + i;
    }
  }
  return result;
}
