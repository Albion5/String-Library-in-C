#include "s21_sscanf.h"

int s21_sscanf(const char *str, const char *format, ...) {
  char beginning[BUF_SIZE] = "";
  Specifier spec_list[BUF_SIZE];
  int spec_amount = get_tokens(beginning, format, spec_list);
  int assign_count = spec_amount;
  if (spec_amount > 0) {
    va_list args;
    va_start(args, format);
    char *cursor = (char *)str;
    assign_count =
        convert_tokens(spec_list, spec_amount, &cursor, args, beginning);
    va_end(args);
  }
  return assign_count;
}

int get_tokens(char *beginning, const char *format, Specifier *spec_list) {
  char *cursor = (char *)format;
  move_cursor(&cursor, 0);
  if (cursor && (cursor > format)) {
    s21_strncpy(beginning, format, cursor - format);
  }
  Token token;
  Specifier specifier;
  init_structs(&token, &specifier);
  int counter = 0;
  while (cursor != s21_NULL && !specifier.error) {
    add_token_info(&token, ++cursor);
    parse_token(token.str, &specifier);
    spec_list[(counter)++] = specifier;
    move_cursor(&cursor, token.end);
  }
  return (specifier.error) ? -1 : counter;
}

void init_structs(Token *token, Specifier *specifier) {
  s21_memset(token->str, 0, BUF_SIZE);
  s21_memset(specifier->add_info, 0, BUF_SIZE);
  specifier->error = 0;
}

void add_token_info(Token *token, char *cursor) {
  int shift = 0;
  if (*cursor == '%') {
    shift++;
  }
  int end = s21_strcspn(cursor + shift, "%");
  token->end = (end) ? end + shift : 1;
  s21_strncpy(token->str, cursor, token->end);
  token->str[token->end] = 0;
}

void parse_token(char *token_str, Specifier *specifier) {
  char *cursor = token_str;
  specifier->width = get_width(&cursor, specifier);
  specifier->len = get_len(&cursor);
  specifier->spec = get_spec(&cursor);
  s21_strcpy(specifier->add_info, cursor);
  specifier->error = (specifier->spec == '?') ? 1 : 0;
}

void move_cursor(char **cursor, int shift) {
  *cursor = s21_strchr(*cursor + shift, '%');
}

char get_len(char **cursor) {
  char symb = '?';
  if (s21_strchr(LENGTH, **cursor)) {
    symb = **cursor;
    if ((**cursor) == 'l' && *(*cursor + 1) == 'l') {
      symb = 'L';
      (*cursor)++;
    }
    (*cursor)++;
  }
  return symb;
}

int get_width(char **cursor, Specifier *specifier) {
  int w = -1, i = 0;
  char width[BUF_SIZE] = "";
  specifier->arg_amount = 1;
  if (**cursor == '*') {
    (specifier->arg_amount)--;
    (*cursor)++;
  }
  get_digital_part(cursor, width, s21_strlen(*cursor), &i);
  if (width[0]) {
    w = atoi(width);
  }
  return w;
}

char get_spec(char **cursor) {
  char symb = '?';
  if (**cursor && s21_strchr(SPECIFIERS, **cursor)) {
    symb = **cursor;
    (*cursor)++;
  }
  return symb;
}

void set_reading_errors(char **cursor, char *str_start, int *i, int amount,
                        int data_match, Specifier *specifier, va_list args,
                        int *n) {
  // Если считывание успешно, строка закончилась, а спецификаторы нет
  if (data_match && **cursor == '\0' && *i < amount) {
    // Присвоить значения следующим спецификаторам n
    if (specifier[*i].spec == 'n') {
      while (*i != amount && (specifier[*i].spec == 'n')) {
        count_chars(cursor, specifier[*i], args, str_start);
        (*i)++;
      }
      //  Если ни одного присваивания еще не было, вернуть ошибку
    } else if (*n == 0) {
      *n = -1;
    }
  }
}

int convert_tokens(Specifier *specifier, int amount, char **cursor,
                   va_list args, char *beginning) {
  char *str_start = *cursor;
  int space = is_prev_space(beginning);
  int skipped = 1, n = 0, i = 0, data_match = 1;
  if (**cursor) {
    skipped = skip_string(cursor, beginning);
  }
  if (!*str_start || !**cursor) {
    if (specifier[0].spec == 'n') {
      while (i != amount && (specifier[i].spec == 'n')) {
        count_chars(cursor, specifier[i], args, str_start);
        i++;
      }
    } else if (skipped) {
      n = -1;
    }
  }
  while (i < amount && **cursor) {
    convert_token_info(specifier[i], cursor, args, &n, str_start, &data_match,
                       space);
    space = is_prev_space(specifier[i++].add_info);
    set_reading_errors(cursor, str_start, &i, amount, data_match, specifier,
                       args, &n);
  }
  return n;
}

void convert_token_info(Specifier specifier, char **cursor, va_list args,
                        int *n, char *str_start, int *data_match, int space) {
  int eof = skip_start(cursor, specifier, space);
  switch (specifier.spec) {
    case 'd':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
    case 'p':
    case 'i':
      *data_match = str_to_int(cursor, n, specifier, args);
      break;
    case 'c':
      *data_match = str_to_char(cursor, n, specifier, args);
      break;
    case 's':
      *data_match = str_to_str(cursor, n, specifier, args);
      break;
    case 'E':
    case 'e':
    case 'f':
    case 'g':
    case 'G':
      *data_match = str_to_float(cursor, n, specifier, args);
      break;
    case '%':
      *data_match = skip_percent(cursor);
      break;
    case 'n':
      count_chars(cursor, specifier, args, str_start);
      break;
  }
  //  Если считывание не удалось из-за конца строки, вернуть ошибку
  if (eof && (*n == 0)) {
    *n = -1;
  }
  if (!eof && *data_match) {
    int skipped = skip_string(cursor, specifier.add_info);
    if ((*n == 0)) {
      //  Если смещение курсора не удалось из-за конца строки
      if (skipped == -1) {
        *n = -1;
        //  Если смещение курсора не удалось по другой причине
      } else if (!skipped) {
        *data_match = 0;
      }
    }
  }
}

int skip_start(char **cursor, Specifier specifier, int space) {
  if ((specifier.spec == 'c' && !space) || specifier.spec == 'n') {
  } else {
    skip_spaces(cursor);
  }
  return **cursor == '\0';
}

int str_to_int(char **cursor, int *count, Specifier specifier, va_list args) {
  char digits[BUF_SIZE] = "";
  specifier.width = set_width(*cursor, specifier.width);
  int i = 0, base = 10;
  get_sign(cursor, digits, specifier.width, &i);
  if (s21_strchr("xXp", specifier.spec)) {
    base = 16;
  } else if (specifier.spec == 'o') {
    base = 8;
  } else if (specifier.spec == 'i') {
    base = get_base(*cursor, specifier.width, i);
  }
  int res = parse_int(cursor, digits, specifier, base, &i);
  if (specifier.arg_amount && res) {
    if (specifier.spec == 'p') {
      void **pointer = va_arg(args, void **);
      *pointer = (void *)strtoul(digits, s21_NULL, base);
      (*count)++;
    } else {
      assign_d_i_o_X_x_u(base, specifier, count, args, digits);
    }
  }
  return res;
}

int str_to_str(char **cursor, int *count, Specifier specifier, va_list args) {
  char chars[BUF_SIZE] = "";
  int end = s21_strcspn(*cursor, " \t\n");
  if (specifier.width <= 0 || specifier.width > end) {
    specifier.width = end;
  }
  int n = parse_chars(cursor, chars, specifier.width);
  if (specifier.arg_amount && n) {
    char *res = (char *)va_arg(args, int *);
    s21_strcpy(res, chars);
    (*count)++;
  }
  return n;
}

int str_to_char(char **cursor, int *count, Specifier specifier, va_list args) {
  char chars[BUF_SIZE] = "";
  if (specifier.width <= 0) {
    specifier.width = 1;
  }
  int n = parse_chars(cursor, chars, specifier.width);
  if (specifier.arg_amount && n) {
    char *res = (char *)va_arg(args, int *);
    s21_strncpy(res, chars, n);
    (*count)++;
  }
  return n;
}

int str_to_float(char **cursor, int *count, Specifier specifier, va_list args) {
  char digits[BUF_SIZE] = {0};
  specifier.width = set_width(*cursor, specifier.width);
  int res = parse_float(cursor, digits, specifier.width);
  if (specifier.arg_amount && res) {
    if (specifier.len == 'L') {
      long double *pointer = va_arg(args, long double *);
      char *end;
      *pointer = strtold(digits, &end);
      (*count)++;
    } else if (specifier.len == 'l') {
      double *pointer = va_arg(args, double *);
      *pointer = atof(digits);
      (*count)++;
    } else {
      float *pointer = va_arg(args, float *);
      *pointer = atof(digits);
      (*count)++;
    }
  }
  return res;
}
int skip_percent(char **cursor) {
  int res = 1;
  if (**cursor == '%') {
    (*cursor)++;
  } else {
    (*cursor) += s21_strlen(*cursor);
    res = 0;
  }
  return res;
}

void count_chars(char **cursor, Specifier specifier, va_list args,
                 const char *start_str) {
  if (*cursor != start_str) {
    skip_spaces(cursor);
  }
  int c = *cursor - start_str;
  if (specifier.arg_amount) {
    int *res = va_arg(args, int *);
    *res = c;
  }
}

int skip_string(char **cursor, char *str) {
  int res = 1;
  if (!**cursor && *str) {
    res = -1;
  }
  while (**cursor && *str && res > 0) {
    if (**cursor == *str) {
      (*cursor)++;
      str++;
    } else if (is_space(*str)) {
      str++;
    } else {
      res = 0;
    }
    if (!**cursor && *str) {
      res = -1;
    }
  }
  if (res != 1) {
    *cursor += s21_strlen(*cursor);
  }
  return res;
}

int is_sign(char c) { return (c == '+' || c == '-'); }
int is_digit(char symb) { return (symb >= '0' && symb <= '9'); }
int is_octal(char symb) { return (symb >= '0' && symb <= '7'); }
int is_hexadecimal(char symb) {
  return is_digit(symb) || (symb >= 'a' && symb <= 'f') ||
         (symb >= 'A' && symb <= 'F');
}

int is_space(char c) { return (c == ' ' || c == '\t' || c == '\n'); }

int is_prev_space(char *str) {
  char *end = (*str) ? str + s21_strlen(str) - 1 : str;
  return is_space(*end);
}

int parse_int(char **cursor, char *digits, Specifier specifier, int base,
              int *i) {
  int digits_count = 0;
  switch (base) {
    case 8:
      digits_count += get_octal_part(cursor, digits, specifier.width, i);
      break;
    case 16:
      digits_count += get_hex_start(cursor, digits, specifier.width, i);
      digits_count += get_hexadecimal_part(cursor, digits, specifier.width, i);
      break;
    case 10:
      digits_count += get_digital_part(cursor, digits, specifier.width, i);
      break;
  }
  if (!digits_count) {
    *cursor += s21_strlen(*cursor);
  }
  return digits_count;
}

int parse_float(char **cursor, char *digits, int width) {
  int i = 0, digits_count = 0;
  get_sign(cursor, digits, width, &i);
  digits_count += get_digital_part(cursor, digits, width, &i);
  digits_count += get_floating_point_part(cursor, digits, width, &i);
  if (digits_count) {
    digits_count += get_exponential_part(cursor, digits, width, &i);
  } else {
    digits_count = get_inf_nan(cursor, digits, width, &i);
  }
  if (!digits_count) {
    (*cursor) += s21_strlen(*cursor);
  }
  return digits_count;
}

int parse_chars(char **cursor, char *chars, int width) {
  int i = 0;
  while (**cursor && i < width) {
    chars[i++] = **cursor;
    (*cursor)++;
  }
  return i;
}

void get_sign(char **cursor, char *digits, int width, int *i) {
  if (is_sign(**cursor) && *i < width) {
    digits[(*i)++] = **cursor;
    (*cursor)++;
  }
}

int get_base(char *cursor, int width, int num) {
  int i = num, base = 10;
  if (*cursor == '0' && i < width) {
    i++;
    base = 8;
    if ((*(cursor + 1) == 'x' || *(cursor + 1) == 'X') && i < width) {
      base = 16;
    }
  }
  return base;
}

int get_digital_part(char **cursor, char *digits, int width, int *i) {
  int digits_count = 0;
  while (**cursor && is_digit(**cursor) && *i < width) {
    digits[(*i)++] = **cursor;
    (*cursor)++;
    digits_count++;
  }
  return digits_count;
}

int get_floating_point_part(char **cursor, char *digits, int width, int *i) {
  int digits_count = 0;
  if (**cursor == '.' && *i < width) {
    digits[(*i)++] = **cursor;
    (*cursor)++;
    digits_count += get_digital_part(cursor, digits, width, i);
  }
  return digits_count;
}

int get_exponential_part(char **cursor, char *digits, int width, int *i) {
  int digits_count = 0;
  if ((**cursor == 'e' || **cursor == 'E') && *i < width) {
    digits[(*i)++] = **cursor;
    (*cursor)++;
    get_sign(cursor, digits, width, i);
    digits_count += get_digital_part(cursor, digits, width, i);
  }
  return digits_count;
}

int get_octal_part(char **cursor, char *digits, int width, int *i) {
  int digits_count = 0;
  while (**cursor && is_octal(**cursor) && *i < width) {
    digits[(*i)++] = **cursor;
    (*cursor)++;
    digits_count++;
  }
  return digits_count;
}

int get_hex_start(char **cursor, char *digits, int width, int *i) {
  int hex_digits_count = 0;
  if (**cursor == '0' && *i < width) {
    digits[(*i)++] = **cursor;
    (*cursor)++;
    hex_digits_count++;
    if ((**cursor == 'x' || **cursor == 'X') && *i < width) {
      digits[(*i)++] = **cursor;
      (*cursor)++;
      hex_digits_count++;
    }
  }
  return hex_digits_count;
}

int get_hexadecimal_part(char **cursor, char *digits, int width, int *i) {
  int hex_digits_count = 0;
  while (**cursor && is_hexadecimal(**cursor) && *i < width) {
    hex_digits_count++;
    digits[(*i)++] = **cursor;
    (*cursor)++;
  }
  return hex_digits_count;
}

int get_inf_nan(char **cursor, char *digits, int width, int *i) {
  int digits_count = 0;
  char *inf = s21_to_lower(*cursor);
  char *str_to_copy = s21_NULL;
  if (((*i + 7) < width) && s21_strncmp("infinity", inf, 8) == 0) {
    str_to_copy = "infinity";
    digits_count += 8;
  } else if (((*i + 2) < width) && s21_strncmp("inf", inf, 3) == 0) {
    str_to_copy = "inf";
    digits_count += 3;
  } else if (((*i + 2) < width) && s21_strncmp("nan", inf, 3) == 0) {
    str_to_copy = "nan";
    digits_count += 3;
  }
  if (str_to_copy) {
    digits[*i] = 0;
    s21_strcat(digits, str_to_copy);
    *i += digits_count;
    (*cursor) += digits_count;
  }
  free(inf);
  return digits_count;
}

void assign_d_i_o_X_x_u(int base, Specifier specifier, int *count, va_list args,
                        char *digits) {
  int sign = 0;
  if (s21_strchr("oxXu", specifier.spec)) {
    sign = 1;
  }
  switch (specifier.len) {
    case '?':
      if (!sign) {
        int *res = va_arg(args, int *);
        *res = strtol(digits, s21_NULL, base);
        (*count)++;
      } else {
        unsigned int *res = va_arg(args, unsigned int *);
        *res = strtoul(digits, s21_NULL, base);
        (*count)++;
      }
      break;
    case 'l':
      if (!sign) {
        long int *res = va_arg(args, long int *);
        *res = strtol(digits, s21_NULL, base);
        (*count)++;
      } else {
        unsigned long int *res = va_arg(args, unsigned long int *);
        *res = strtoul(digits, s21_NULL, base);
        (*count)++;
      }
      break;
    case 'L':
      if (!sign) {
        long long int *res = va_arg(args, long long int *);
        *res = strtoll(digits, s21_NULL, base);
        (*count)++;
      } else {
        unsigned long long int *res = va_arg(args, unsigned long long int *);
        *res = strtoull(digits, s21_NULL, base);
        (*count)++;
      }
      break;
    case 'h':
      if (!sign) {
        short int *res = va_arg(args, short int *);
        *res = strtol(digits, s21_NULL, base);
        (*count)++;
      } else {
        unsigned short int *res = va_arg(args, unsigned short int *);
        *res = strtoul(digits, s21_NULL, base);
        (*count)++;
      }
      break;
  }
}

int set_width(char *cursor, int width) {
  return (width <= 0) ? (int)s21_strlen(cursor) : width;
}
void skip_spaces(char **cursor) { *cursor += s21_strspn(*cursor, " \t\n"); }
