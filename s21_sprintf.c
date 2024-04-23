#include "s21_sprintf.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  sprintf_result result = {str, 0};
  char *format_ptr = (char *)format;
  while (*format_ptr && result.len != -1) {
    if (*format_ptr == '%') {
      format_ptr++;
      specifier_handler(&result, &format_ptr, &ap);
    } else {
      *(result.str++) = *(format_ptr++);
      result.len++;
    }
    *(result.str) = 0;
  }
  va_end(ap);
  return result.len;
}

void specifier_handler(sprintf_result *result, char **format_ptr, va_list *ap) {
  specifier specifier = {{0, 0, 0, 0, 0}, -1, -1, 0, 0};
  s21_size_t spec_substr_len = s21_strcspn(*format_ptr, "cdieEfgGosuxXpn%");
  specifier.specifier = *(*format_ptr + spec_substr_len);
  if (specifier.specifier &&
      parse_specifier(format_ptr, ap, &specifier) == SUCCESS) {
    if (specifier.specifier == 'n') {
      *(va_arg(*ap, int *)) = specifier.length == 'l'   ? (long int)result->len
                              : specifier.length == 'h' ? (short int)result->len
                                                        : result->len;
    } else {
      add_specified(result, ap, &specifier);
    }
  } else {
    result->len = -1;
  }
}

status parse_specifier(char **format_ptr, va_list *ap, specifier *specifier) {
  parse_specifier_flags(format_ptr, specifier);
  parse_specifier_width(format_ptr, ap, specifier);
  if (**format_ptr == '.') {
    (*format_ptr)++;
    parse_specifier_precision(format_ptr, ap, specifier);
    if (s21_strchr("diouxX", specifier->specifier) &&
        specifier->precision != -1)
      specifier->flags.null = 0;
  }
  parse_specifier_length(format_ptr, specifier);
  status parse_status = SUCCESS;
  if (*(*format_ptr)++ != specifier->specifier) parse_status = FAIL;
  return parse_status;
}

void parse_specifier_flags(char **format_ptr, specifier *specifier) {
  char *tmp, *sprintf_flags_str_ptr = SPINTF_FLAGS_STR;
  while ((tmp = s21_strchr(sprintf_flags_str_ptr, **format_ptr))) {
    *((int *)&(specifier->flags) + (tmp - sprintf_flags_str_ptr)) = 1;
    (*format_ptr)++;
  }
  if (specifier->flags.minus) specifier->flags.null = 0;
  if (specifier->flags.plus) specifier->flags.space = 0;
}

void parse_specifier_width(char **format_ptr, va_list *ap,
                           specifier *specifier) {
  if (**format_ptr == '*') {
    specifier->width = va_arg(*ap, int);
    (*format_ptr)++;
  } else if (s21_strchr(NUMBERS_STR, **format_ptr)) {
    specifier->width = 0;
    while ((s21_strchr(NUMBERS_STR, **format_ptr))) {
      specifier->width =
          specifier->width * 10 + **format_ptr - OFF_NUMBER_TO_CHAR;
      (*format_ptr)++;
    }
  }
}

void parse_specifier_precision(char **format_ptr, va_list *ap,
                               specifier *specifier) {
  if (**format_ptr == '*') {
    specifier->precision = va_arg(*ap, int);
    (*format_ptr)++;
  } else {
    specifier->precision = 0;
    while ((s21_strchr(NUMBERS_STR, **format_ptr))) {
      specifier->precision =
          specifier->precision * 10 + **format_ptr - OFF_NUMBER_TO_CHAR;
      (*format_ptr)++;
    }
  }
}

void parse_specifier_length(char **format_ptr, specifier *specifier) {
  if (s21_strchr(SPINTF_LENGTH_STR, **format_ptr))
    specifier->length = *(*format_ptr)++;
}

void add_specified(sprintf_result *result, va_list *ap, specifier *specifier) {
  char spec = specifier->specifier;
  if (spec == 'c') add_c(result, ap, specifier);
  if (spec == 'd' || spec == 'i') add_di(result, ap, specifier);
  if (spec == 'e' || spec == 'E') add_eE(result, ap, specifier);
  if (spec == 'f') add_f(result, ap, specifier);
  if (spec == 'g' || spec == 'G') add_gG(result, ap, specifier);
  if (spec == 'o') add_o(result, ap, specifier);
  if (spec == 's') add_s(result, ap, specifier);
  if (spec == 'u') add_u(result, ap, specifier);
  if (spec == 'x' || spec == 'X') add_xX(result, ap, specifier);
  if (spec == 'p') add_p(result, ap, specifier);
  if (spec == '%') add_percent(result);
}

void add_c(sprintf_result *result, va_list *ap, specifier *specifier) {
  if (specifier->width < 1) specifier->width = 1;
  if (!specifier->flags.minus && specifier->width > 1)
    supplement_str(result, ' ', specifier->width - 1);
  *(result->str)++ = (char)va_arg(*ap, int);
  result->len++;
  if (specifier->flags.minus && specifier->width > 1)
    supplement_str(result, ' ', specifier->width - 1);
}

void supplement_str(sprintf_result *result, char chr, int count) {
  while (count--) {
    *(result->str)++ = chr;
    result->len++;
  }
}

void add_s(sprintf_result *result, va_list *ap, specifier *specifier) {
  char *str_to_add = va_arg(*ap, char *);
  add_str(result, str_to_add, specifier);
}

void add_str(sprintf_result *result, char *str, specifier *specifier) {
  int str_len = (int)s21_strlen(str);
  int str_to_add_len = specifier->precision < 0         ? str_len
                       : specifier->precision < str_len ? specifier->precision
                                                        : str_len;
  int to_supplement = specifier->width - str_to_add_len;
  if (!specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
  while (*str && str_to_add_len--) {
    *(result->str)++ = *str++;
    result->len++;
  }
  if (specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
}

void add_uncut_str(sprintf_result *result, char *str, specifier *specifier) {
  specifier->precision = -1;
  add_str(result, str, specifier);
}

void add_simple_str(sprintf_result *result, char *str) {
  while (*str) {
    *(result->str)++ = *str++;
    result->len++;
  }
}

void add_percent(sprintf_result *result) {
  *(result->str)++ = '%';
  result->len++;
}

void add_di(sprintf_result *result, va_list *ap, specifier *specifier) {
  long int num = specifier->length == 'l'   ? va_arg(*ap, long int)
                 : specifier->length == 'h' ? (short int)va_arg(*ap, int)
                                            : va_arg(*ap, int);
  int num_is_negative = num < 0 ? (num = -num) : 0;
  int num_len = get_number_len(num);
  int need_sign = 0;
  if (specifier->precision == -1) specifier->precision = 1;
  if (specifier->flags.plus || specifier->flags.space || num_is_negative)
    need_sign = 1;
  if (specifier->flags.null)
    specifier->precision =
        max(specifier->precision, specifier->width - need_sign);
  int len_to_add = max(specifier->precision, num_len);
  int to_supplement = specifier->width - len_to_add - need_sign;
  if (!specifier->flags.minus && to_supplement > 0) {
    supplement_str(result, ' ', to_supplement);
  }
  if (need_sign) add_sign(result, specifier, num_is_negative);
  add_int_num(result, num, len_to_add);
  if (specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
}

int max(int a, int b) { return a > b ? a : b; }

int get_number_len(unsigned long int num) {
  int len = 0;
  while (num) {
    num /= 10;
    len++;
  }
  return len;
}

void add_sign(sprintf_result *result, specifier *specifier, int is_negative) {
  *(result->str)++ = is_negative ? '-' : specifier->flags.plus ? '+' : ' ';
  result->len++;
}

void add_int_num(sprintf_result *result, unsigned long num, int len_to_add) {
  for (int i = len_to_add - 1; i >= 0; i--) {
    *(result->str + i) = num % 10 + OFF_NUMBER_TO_CHAR;
    num /= 10;
  }
  (result->str) += len_to_add;
  result->len += len_to_add;
}

void add_u(sprintf_result *result, va_list *ap, specifier *specifier) {
  unsigned long int num =
      specifier->length == 'l'   ? va_arg(*ap, unsigned long int)
      : specifier->length == 'h' ? (unsigned short int)va_arg(*ap, unsigned int)
                                 : va_arg(*ap, unsigned int);
  int num_len = get_number_len(num);
  if (specifier->precision == -1) specifier->precision = 1;
  if (specifier->flags.null)
    specifier->precision = max(specifier->precision, specifier->width);
  int len_to_add = max(specifier->precision, num_len);
  int to_supplement = specifier->width - len_to_add;
  if (!specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
  add_int_num(result, num, len_to_add);
  if (specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
}

void add_xX(sprintf_result *result, va_list *ap, specifier *specifier) {
  unsigned long int num =
      specifier->length == 'l'   ? va_arg(*ap, unsigned long int)
      : specifier->length == 'h' ? (unsigned short int)va_arg(*ap, unsigned int)
                                 : va_arg(*ap, unsigned int);
  int num_len = get_hex_number_len(num);
  int need_0x = specifier->flags.hash && num ? 2 : 0;
  if (specifier->precision == -1) specifier->precision = 1;
  if (specifier->flags.null)
    specifier->precision =
        max(specifier->precision, specifier->width - need_0x);
  int len_to_add = max(specifier->precision, num_len);
  int to_supplement = specifier->width - len_to_add - need_0x;
  if (!specifier->flags.minus && to_supplement > 0) {
    supplement_str(result, ' ', to_supplement);
  }
  if (need_0x)
    add_simple_str(result, specifier->specifier == 'x' ? "0x" : "0X");
  for (int i = len_to_add - 1; i >= 0; i--) {
    int val_to_add = num % 16;
    *(result->str + i) = val_to_add < 10               ? val_to_add + '0'
                         : specifier->specifier == 'x' ? val_to_add - 10 + 'a'
                                                       : val_to_add - 10 + 'A';
    num /= 16;
  }
  (result->str) += len_to_add;
  result->len += len_to_add;
  if (specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
}

int get_hex_number_len(unsigned long num) {
  int len = 0;
  while (num) {
    num /= 16;
    len++;
  }
  return len;
}

void add_o(sprintf_result *result, va_list *ap, specifier *specifier) {
  unsigned long int num =
      specifier->length == 'l'   ? va_arg(*ap, unsigned long int)
      : specifier->length == 'h' ? (unsigned short int)va_arg(*ap, unsigned int)
                                 : va_arg(*ap, unsigned int);
  int num_len = get_oct_number_len(num);
  int need_0 =
      specifier->flags.hash && num && specifier->precision < num_len ? 1 : 0;
  if (specifier->precision == -1) specifier->precision = 1;
  if (specifier->flags.null)
    specifier->precision = max(specifier->precision, specifier->width - need_0);
  int len_to_add = max(specifier->precision, num_len);
  int to_supplement = specifier->width - len_to_add - need_0;
  if (!specifier->flags.minus && to_supplement > 0) {
    supplement_str(result, ' ', to_supplement);
  }
  if (need_0) add_simple_str(result, "0");
  for (int i = len_to_add - 1; i >= 0; i--) {
    *(result->str + i) = num % 8 + '0';
    num /= 8;
  }
  (result->str) += len_to_add;
  result->len += len_to_add;
  if (specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
}

int get_oct_number_len(unsigned long num) {
  int len = 0;
  while (num) {
    num /= 8;
    len++;
  }
  return len;
}

void add_p(sprintf_result *result, va_list *ap, specifier *specifier) {
  void *ptr = va_arg(*ap, void *);
  if (ptr == s21_NULL) {
#ifdef __unix__
    char *null_str = "(nil)";
#endif
#ifdef __APPLE__
    char *null_str = "0x0";
#endif
    add_str(result, null_str, specifier);
  } else {
    unsigned long num = (unsigned long)ptr;
    int len_to_add = get_hex_number_len(num);
    int to_supplement = specifier->width - len_to_add - 2;
    if (!specifier->flags.minus && to_supplement > 0) {
      supplement_str(result, ' ', to_supplement);
    }
    add_simple_str(result, "0x");
    for (int i = len_to_add - 1; i >= 0; i--) {
      int val_to_add = num % 16;
      *(result->str + i) =
          val_to_add < 10 ? val_to_add + '0' : val_to_add - 10 + 'a';
      num /= 16;
    }
    (result->str) += len_to_add;
    result->len += len_to_add;
    if (specifier->flags.minus && to_supplement > 0)
      supplement_str(result, ' ', to_supplement);
  }
}

void add_f(sprintf_result *result, va_list *ap, specifier *specifier) {
  long double num =
      specifier->length == 'L' ? va_arg(*ap, long double) : va_arg(*ap, double);
  if (isnan(num)) {
#ifdef __unix__
    if (signbit(num)) {
      add_uncut_str(result, "-nan", specifier);
    } else {
      add_uncut_str(result, "nan", specifier);
    }
#endif
#ifdef __APPLE__
    add_uncut_str(result, "nan", specifier);
#endif
  } else if (num > __DBL_MAX__) {
    add_uncut_str(result, "inf", specifier);
  } else if (num < -__DBL_MAX__) {
    add_uncut_str(result, "-inf", specifier);
  } else {
    add_real(result, num, specifier, NO_TRIM_TRAILING_ZEROS);
  }
}

void add_real(sprintf_result *result, long double num, specifier *specifier,
              int need_trim_trailing_zeros) {
  int num_is_negative = num < 0 ? ceil(num = -num) : 0;
  int num_fractional_part_len =
      specifier->precision == -1 ? 6 : specifier->precision;
  long double num_integer_part = 0, num_fractional_part = 0;
  num += 0.5L * powl(10, -num_fractional_part_len);
  num_fractional_part = modfl(num, &num_integer_part);
  if (need_trim_trailing_zeros)
    num_fractional_part_len = get_fract_len_no_trail_zeros(
        num_fractional_part, num_fractional_part_len);
  int need_sign = 0, need_dot = 0;
  if (specifier->flags.plus || specifier->flags.space || num_is_negative)
    need_sign = 1;
  if (num_fractional_part_len || specifier->flags.hash) need_dot = 1;
  int num_integer_part_len = get_nonint_number_len(num_integer_part);
  if (num_integer_part_len == 0) num_integer_part_len = 1;
  if (specifier->flags.null)
    num_integer_part_len =
        max(num_integer_part_len,
            specifier->width - need_sign - num_fractional_part_len - need_dot);
  int len_to_add =
      need_sign + num_integer_part_len + need_dot + num_fractional_part_len;
  int to_supplement = specifier->width - len_to_add;
  if (!specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
  if (need_sign) add_sign(result, specifier, num_is_negative);
  add_real_int(result, num_integer_part, num_integer_part_len);
  if (need_dot) add_dot(result);
  add_real_fract(result, num_fractional_part, num_fractional_part_len);
  if (specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
}

int get_fract_len_no_trail_zeros(long double fractional, int fractional_len) {
  int len = fractional_len;
  long double tmp = floor(fractional * powl(10, fractional_len));
  while (fmodl(tmp, 10) < 0.000001 && len) {
    tmp /= 10;
    len--;
  }
  return len;
}

void add_real_int(sprintf_result *result, long double real_int, int len) {
  for (int i = len - 1; i >= 0; i--) {
    *(result->str + i) =
        floorl(fmodl(real_int, powl(10L, len - i)) / powl(10L, len - i - 1)) +
        OFF_NUMBER_TO_CHAR;
  }
  (result->str) += len;
  result->len += len;
}

void add_dot(sprintf_result *result) {
  *(result->str)++ = '.';
  result->len++;
}

void add_real_fract(sprintf_result *result, long double real_fract, int len) {
  for (int i = 0; i < len; i++) {
    long double tmp;
    *(result->str)++ = floorl(real_fract *= 10) + OFF_NUMBER_TO_CHAR;
    real_fract = modfl(real_fract, &tmp);
  }
  result->len += len;
}

int get_nonint_number_len(long double num) {
  int len = 0;
  while (num) {
    num = floorl(num / 10L);
    len++;
  }
  return len;
}

void add_eE(sprintf_result *result, va_list *ap, specifier *specifier) {
  long double num =
      specifier->length == 'L' ? va_arg(*ap, long double) : va_arg(*ap, double);
  if (isnan(num)) {
#ifdef __unix__
    if (signbit(num)) {
      add_uncut_str(result, specifier->specifier == 'e' ? "-nan" : "-NAN",
                    specifier);
    } else {
      add_uncut_str(result, specifier->specifier == 'e' ? "nan" : "NAN",
                    specifier);
    }
#endif
#ifdef __APPLE__
    add_uncut_str(result, specifier->specifier == 'e' ? "nan" : "NAN",
                  specifier);
#endif
  } else if (num > __DBL_MAX__) {
    add_uncut_str(result, specifier->specifier == 'e' ? "inf" : "INF",
                  specifier);
  } else if (num < -__DBL_MAX__) {
    add_uncut_str(result, specifier->specifier == 'e' ? "-inf" : "-INF",
                  specifier);
  } else {
    add_exponential(result, num, specifier, NO_TRIM_TRAILING_ZEROS);
  }
}

void add_exponential(sprintf_result *result, long double num,
                     specifier *specifier, int need_trim_trailing_zeros) {
  int num_is_negative = num < 0 ? ceil(num = -num) : 0;
  int num_fractional_part_len =
      specifier->precision == -1 ? 6 : specifier->precision;
  int need_sign = 0, need_dot = 0;
  if (specifier->flags.plus || specifier->flags.space || num_is_negative)
    need_sign = 1;
  int exponent = 0;
  long double num_integer_part = 0, num_fractional_part = 0;
  num = get_normalized_num(num, &exponent) +
        0.5L * powl(10, -num_fractional_part_len);
  if (num) num_fractional_part = modfl(num, &num_integer_part);
  if (need_trim_trailing_zeros)
    num_fractional_part_len = get_fract_len_no_trail_zeros(
        num_fractional_part, num_fractional_part_len);
  if (num_fractional_part_len || specifier->flags.hash) need_dot = 1;
  int is_exp_neg = exponent < 0 ? (exponent = -exponent) : 0;
  int exp_full_len = 0, exp_len = get_number_len(exponent);
  exp_len = max(exp_len, 2);
  exp_full_len = exp_len + 2;
  int num_integer_part_len = 1;
  if (specifier->flags.null)
    num_integer_part_len =
        max(num_integer_part_len, specifier->width - need_sign -
                                      num_fractional_part_len - need_dot -
                                      exp_full_len);
  int len_to_add = need_sign + num_integer_part_len + need_dot +
                   num_fractional_part_len + exp_full_len;
  int to_supplement = specifier->width - len_to_add;
  if (!specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
  if (need_sign) add_sign(result, specifier, num_is_negative);
  add_real_int(result, num_integer_part, num_integer_part_len);
  if (need_dot) add_dot(result);
  add_real_fract(result, num_fractional_part, num_fractional_part_len);
  add_simple_str(result, specifier->specifier == 'e' ? "e" : "E");
  add_simple_str(result, is_exp_neg ? "-" : "+");
  add_int_num(result, exponent, exp_len);
  if (specifier->flags.minus && to_supplement > 0)
    supplement_str(result, ' ', to_supplement);
}

long double get_normalized_num(long double num, int *exponent) {
  *exponent = 0;
  normalize_real(&num, exponent);
  return num;
}

int get_exponent(long double num) {
  int exponent = 0;
  normalize_real(&num, &exponent);
  return exponent;
}

void normalize_real(long double *num, int *exponent) {
  if (*num < 0) *num = -*num;
  if (*num) {
    while (*num >= 10) {
      *num /= 10;
      (*exponent)++;
    }
    while (*num < 1) {
      *num *= 10;
      (*exponent)--;
    }
  }
}

void add_gG(sprintf_result *result, va_list *ap, specifier *specifier) {
  long double num =
      specifier->length == 'L' ? va_arg(*ap, long double) : va_arg(*ap, double);
  if (isnan(num)) {
#ifdef __unix__
    if (signbit(num)) {
      add_uncut_str(result, specifier->specifier == 'g' ? "-nan" : "-NAN",
                    specifier);
    } else {
      add_uncut_str(result, specifier->specifier == 'g' ? "nan" : "NAN",
                    specifier);
    }
#endif
#ifdef __APPLE__
    add_uncut_str(result, specifier->specifier == 'g' ? "nan" : "NAN",
                  specifier);
#endif
  } else if (num > __DBL_MAX__) {
    add_uncut_str(result, specifier->specifier == 'g' ? "inf" : "INF",
                  specifier);
  } else if (num < -__DBL_MAX__) {
    add_uncut_str(result, specifier->specifier == 'g' ? "-inf" : "-INF",
                  specifier);
  } else {
    specifier->precision = specifier->precision == -1  ? 6
                           : specifier->precision == 0 ? 1
                                                       : specifier->precision;
    // printf("num:%Lf\n", num);
    int exponent = get_exponent(num);
    // printf("exp:%d\n", exponent);
    int need_trim_trailing_zeros =
        specifier->flags.hash ? NO_TRIM_TRAILING_ZEROS : TRIM_TRAILING_ZEROS;
    if (exponent < specifier->precision && exponent >= -4) {
      specifier->precision -= (exponent + 1);
      add_real(result, num, specifier, need_trim_trailing_zeros);
    } else {
      specifier->precision -= 1;
      specifier->specifier = specifier->specifier == 'g' ? 'e' : 'E';
      add_exponential(result, num, specifier, need_trim_trailing_zeros);
    }
  }
}
