#ifndef C2_S21_STRINGPLUS_S21_SSCANF_H_
#define C2_S21_STRINGPLUS_S21_SSCANF_H_

#include <stdarg.h>
#include <stdlib.h>

#include "s21_string.h"

#define SPECIFIERS "cdieEfgGosuxXpn%"
#define LENGTH "hlL"
#define BUF_SIZE 200

typedef struct Specifier {
  int width;
  char len;
  char spec;
  int arg_amount;
  char add_info[BUF_SIZE];
  int error;
} Specifier;

typedef struct Token {
  int end;
  char str[BUF_SIZE];
} Token;

void set_reading_errors(char **cursor, char *str_start, int *i, int amount,
                        int data_match, Specifier *specifier, va_list args,
                        int *n);
char get_spec(char **cursor);
char get_len(char **cursor);
int is_sign(char c);
void init_structs(Token *token, Specifier *specifier);
void move_cursor(char **cursor, int shift);
int is_space(char c);
void skip_spaces(char **cursor);
int skip_string(char **cursor, char *str);
int skip_percent(char **cursor);
int get_digital_part(char **cursor, char *digits, int width, int *i);
int get_width(char **cursor, Specifier *specifier);
int set_width(char *cursor, int width);
int parse_chars(char **cursor, char *chars, int width);
int str_to_str(char **cursor, int *count, Specifier specifier, va_list args);
int str_to_char(char **cursor, int *count, Specifier specifier, va_list args);
void get_sign(char **cursor, char *digits, int width, int *i);
int get_inf_nan(char **cursor, char *digits, int width, int *i);
int get_floating_point_part(char **cursor, char *digits, int width, int *i);
int get_exponential_part(char **cursor, char *digits, int width, int *i);
int parse_float(char **cursor, char *digits, int width);
int str_to_float(char **cursor, int *count, Specifier specifier, va_list args);
int get_hex_start(char **cursor, char *digits, int width, int *i);
int get_hexadecimal_part(char **cursor, char *digits, int width, int *i);
int get_octal_part(char **cursor, char *digits, int width, int *i);
int parse_int(char **cursor, char *digits, Specifier specifier, int base,
              int *i);
int get_base(char *cursor, int width, int num);
void assign_d_i_o_X_x_u(int base, Specifier specifier, int *count, va_list args,
                        char *digits);
int str_to_int(char **cursor, int *count, Specifier specifier, va_list args);
void count_chars(char **cursor, Specifier specifier, va_list args,
                 const char *start_str);
int is_prev_space(char *str);
int skip_start(char **cursor, Specifier specifier, int space);
void convert_token_info(Specifier specifier, char **cursor, va_list args,
                        int *n, char *str_start, int *data_match, int space);
int convert_tokens(Specifier *specifier, int amount, char **cursor,
                   va_list args, char *beginning);
void parse_token(char *token_str, Specifier *specifier);
void add_token_info(Token *token, char *cursor);
int get_tokens(char *beginning, const char *format, Specifier *spec_list);
int s21_sscanf(const char *str, const char *format, ...);

#endif  // C2_S21_STRINGPLUS_S21_SSCANF_H_
