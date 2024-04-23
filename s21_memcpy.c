#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *dest_ptr = (unsigned char *)dest,
                *src_ptr = (unsigned char *)src;
  while (n--) {
    *(dest_ptr++) = *(src_ptr++);
  }
  return dest;
}
