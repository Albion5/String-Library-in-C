#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *src_trimmed = s21_NULL;
  char *src_ptr = (char *)src;
  char *trim_chars_ptr = (char *)trim_chars;
  if (trim_chars_ptr == s21_NULL || *trim_chars_ptr == 0) trim_chars_ptr = " ";
  int src_trimmed_len = 0;
  s21_size_t trim_start_len = 0;
  if (src) {
    s21_size_t src_len = s21_strlen(src);
    int trim_end_len = 0;
    while (*src_ptr && s21_strchr(trim_chars_ptr, *src_ptr++)) {
      trim_start_len++;
    }
    src_ptr = (char *)src + src_len - 1;
    while (src_ptr >= src && s21_strchr(trim_chars_ptr, *src_ptr--)) {
      trim_end_len++;
    }
    src_trimmed_len = src_len - trim_start_len - trim_end_len;
    if (src_trimmed_len < 0) src_trimmed_len = 0;
    src_trimmed = (char *)malloc((src_trimmed_len + 1) * sizeof(char));
  }
  if (src_trimmed) {
    src_ptr = (char *)src + trim_start_len;
    s21_strncpy(src_trimmed, src_ptr, src_trimmed_len);
    *(src_trimmed + src_trimmed_len) = 0;
  }
  return src_trimmed;
}
