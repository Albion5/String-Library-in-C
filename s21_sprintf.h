#ifndef C2_S21_STRINGPLUS_S21_SPRINTF_H_
#define C2_S21_STRINGPLUS_S21_SPRINTF_H_

#include <stdarg.h>

#include "s21_string.h"

#define SPINTF_FLAGS_STR "-+ #0"
#define SPINTF_LENGTH_STR "hlL"
#define NUMBERS_STR "1234567890"
#define OFF_NUMBER_TO_CHAR 48
#define TRIM_TRAILING_ZEROS 1
#define NO_TRIM_TRAILING_ZEROS 0

typedef enum status { SUCCESS, FAIL } status;

typedef struct specifier_flags {
  int minus;
  int plus;
  int space;
  int hash;
  int null;
} specifier_flags;

typedef struct specifier {
  specifier_flags flags;
  int width;
  int precision;
  char length;
  char specifier;
} specifier;

typedef struct sprintf_result {
  char *str;
  int len;
} sprintf_result;

void specifier_handler(sprintf_result *result, char **format_ptr, va_list *ap);
status parse_specifier(char **format_ptr, va_list *ap, specifier *specifier);
void parse_specifier_flags(char **format_ptr, specifier *specifier);
void parse_specifier_width(char **format_ptr, va_list *ap,
                           specifier *specifier);
void parse_specifier_precision(char **format_ptr, va_list *ap,
                               specifier *specifier);
void parse_specifier_length(char **format_ptr, specifier *specifier);
void add_specified(sprintf_result *result, va_list *ap, specifier *specifier);

void add_c(sprintf_result *result, va_list *ap, specifier *specifier);
void supplement_str(sprintf_result *result, char chr, int count);
void add_s(sprintf_result *result, va_list *ap, specifier *specifier);
void add_str(sprintf_result *result, char *str, specifier *specifier);
void add_uncut_str(sprintf_result *result, char *str, specifier *specifier);
void add_simple_str(sprintf_result *result, char *str);
void add_percent(sprintf_result *result);
void add_di(sprintf_result *result, va_list *ap, specifier *specifier);
int max(int a, int b);
int get_number_len(unsigned long int num);
void add_sign(sprintf_result *result, specifier *specifier, int is_negative);
void add_int_num(sprintf_result *result, unsigned long num, int len_to_add);
void add_u(sprintf_result *result, va_list *ap, specifier *specifier);
void add_xX(sprintf_result *result, va_list *ap, specifier *specifier);
int get_hex_number_len(unsigned long num);
void add_o(sprintf_result *result, va_list *ap, specifier *specifier);
int get_oct_number_len(unsigned long num);
void add_p(sprintf_result *result, va_list *ap, specifier *specifier);
void add_f(sprintf_result *result, va_list *ap, specifier *specifier);
void add_real(sprintf_result *result, long double num, specifier *specifier,
              int need_trim_trailing_zeros);
int get_fract_len_no_trail_zeros(long double fractional, int fractional_len);
void add_real_int(sprintf_result *result, long double real_int, int len);
void add_dot(sprintf_result *result);
void add_real_fract(sprintf_result *result, long double real_fract, int len);
int get_nonint_number_len(long double num);
void add_eE(sprintf_result *result, va_list *ap, specifier *specifier);
void add_exponential(sprintf_result *result, long double num,
                     specifier *specifier, int need_trim_trailing_zeros);
long double get_normalized_num(long double num, int *exponent);
int get_exponent(long double num);
void normalize_real(long double *num, int *exponent);
void add_gG(sprintf_result *result, va_list *ap, specifier *specifier);

#endif  // C2_S21_STRINGPLUS_S21_SPRINTF_H_
