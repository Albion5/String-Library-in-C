#include "s21_string.h"

char *unknown_error_handler(int errnum);

char *s21_strerror(int errnum) {
  char *result;
  if (errnum < 0 || errnum > MAX_ERROR_INDEX) {
    result = unknown_error_handler(errnum);
  } else {
    result = err_list[errnum];
  }
  return result;
}

char *unknown_error_handler(int errnum) {
  static char err_mess[40];
  s21_strcpy(err_mess, ERR_MESS_PATTERN);
  char buffer[20];
  char *err_mess_ptr = err_mess + s21_strlen(err_mess), *buffer_ptr = buffer;
  if (errnum < 0) {
    errnum = -errnum;
    *err_mess_ptr++ = '-';
  }
  while (errnum) {
    *buffer_ptr++ = errnum % 10 + '0';
    errnum /= 10;
  }
  while (buffer_ptr >= buffer) {
    *err_mess_ptr++ = *--buffer_ptr;
  }
  *err_mess_ptr = 0;
  return err_mess;
}
