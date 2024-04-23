#ifndef C2_S21_STRINGPLUS_S21_STRING_H_
#define C2_S21_STRINGPLUS_S21_STRING_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define s21_NULL (void *)0
typedef unsigned long long int s21_size_t;

#ifdef __APPLE__
#define err_list                                                               \
  (char *[]) {                                                                 \
    "Undefined error: 0", "Operation not permitted",                           \
        "No such file or directory", "No such process",                        \
        "Interrupted system call", "Input/output error",                       \
        "Device not configured", "Argument list too long",                     \
        "Exec format error", "Bad file descriptor", "No child processes",      \
        "Resource deadlock avoided", "Cannot allocate memory",                 \
        "Permission denied", "Bad address", "Block device required",           \
        "Resource busy", "File exists", "Cross-device link",                   \
        "Operation not supported by device", "Not a directory",                \
        "Is a directory", "Invalid argument", "Too many open files in system", \
        "Too many open files", "Inappropriate ioctl for device",               \
        "Text file busy", "File too large", "No space left on device",         \
        "Illegal seek", "Read-only file system", "Too many links",             \
        "Broken pipe", "Numerical argument out of domain", "Result too large", \
        "Resource temporarily unavailable", "Operation now in progress",       \
        "Operation already in progress", "Socket operation on non-socket",     \
        "Destination address required", "Message too long",                    \
        "Protocol wrong type for socket", "Protocol not available",            \
        "Protocol not supported", "Socket type not supported",                 \
        "Operation not supported", "Protocol family not supported",            \
        "Address family not supported by protocol family",                     \
        "Address already in use", "Can't assign requested address",            \
        "Network is down", "Network is unreachable",                           \
        "Network dropped connection on reset",                                 \
        "Software caused connection abort", "Connection reset by peer",        \
        "No buffer space available", "Socket is already connected",            \
        "Socket is not connected", "Can't send after socket shutdown",         \
        "Too many references: can't splice", "Operation timed out",            \
        "Connection refused", "Too many levels of symbolic links",             \
        "File name too long", "Host is down", "No route to host",              \
        "Directory not empty", "Too many processes", "Too many users",         \
        "Disc quota exceeded", "Stale NFS file handle",                        \
        "Too many levels of remote in path", "RPC struct is bad",              \
        "RPC version wrong", "RPC prog. not avail", "Program version wrong",   \
        "Bad procedure for program", "No locks available",                     \
        "Function not implemented", "Inappropriate file type or format",       \
        "Authentication error", "Need authenticator", "Device power is off",   \
        "Device error", "Value too large to be stored in data type",           \
        "Bad executable (or shared library)", "Bad CPU type in executable",    \
        "Shared library version mismatch", "Malformed Mach-o file",            \
        "Operation canceled", "Identifier removed",                            \
        "No message of desired type", "Illegal byte sequence",                 \
        "Attribute not found", "Bad message", "EMULTIHOP (Reserved)",          \
        "No message available on STREAM", "ENOLINK (Reserved)",                \
        "No STREAM resources", "Not a STREAM", "Protocol error",               \
        "STREAM ioctl timeout", "Operation not supported on socket",           \
        "Policy not found", "State not recoverable", "Previous owner died",    \
        "Interface output queue is full"                                       \
  }
/*further down the list: "Unknown error: 107" etc.*/
#define MAX_ERROR_INDEX 106
#define ERR_MESS_PATTERN "Unknown error: "
#endif

#ifdef __unix__
#define err_list                                                               \
  (char *[]) {                                                                 \
    "Success", "Operation not permitted", "No such file or directory",         \
        "No such process", "Interrupted system call", "Input/output error",    \
        "No such device or address", "Argument list too long",                 \
        "Exec format error", "Bad file descriptor", "No child processes",      \
        "Resource temporarily unavailable", "Cannot allocate memory",          \
        "Permission denied", "Bad address", "Block device required",           \
        "Device or resource busy", "File exists", "Invalid cross-device link", \
        "No such device", "Not a directory", "Is a directory",                 \
        "Invalid argument", "Too many open files in system",                   \
        "Too many open files", "Inappropriate ioctl for device",               \
        "Text file busy", "File too large", "No space left on device",         \
        "Illegal seek", "Read-only file system", "Too many links",             \
        "Broken pipe", "Numerical argument out of domain",                     \
        "Numerical result out of range", "Resource deadlock avoided",          \
        "File name too long", "No locks available",                            \
        "Function not implemented", "Directory not empty",                     \
        "Too many levels of symbolic links", "Unknown error 41",               \
        "No message of desired type", "Identifier removed",                    \
        "Channel number out of range", "Level 2 not synchronized",             \
        "Level 3 halted", "Level 3 reset", "Link number out of range",         \
        "Protocol driver not attached", "No CSI structure available",          \
        "Level 2 halted", "Invalid exchange", "Invalid request descriptor",    \
        "Exchange full", "No anode", "Invalid request code", "Invalid slot",   \
        "Unknown error 58", "Bad font file format", "Device not a stream",     \
        "No data available", "Timer expired", "Out of streams resources",      \
        "Machine is not on the network", "Package not installed",              \
        "Object is remote", "Link has been severed", "Advertise error",        \
        "Srmount error", "Communication error on send", "Protocol error",      \
        "Multihop attempted", "RFS specific error", "Bad message",             \
        "Value too large for defined data type", "Name not unique on network", \
        "File descriptor in bad state", "Remote address changed",              \
        "Can not access a needed shared library",                              \
        "Accessing a corrupted shared library",                                \
        ".lib section in a.out corrupted",                                     \
        "Attempting to link in too many shared libraries",                     \
        "Cannot exec a shared library directly",                               \
        "Invalid or incomplete multibyte or wide character",                   \
        "Interrupted system call should be restarted", "Streams pipe error",   \
        "Too many users", "Socket operation on non-socket",                    \
        "Destination address required", "Message too long",                    \
        "Protocol wrong type for socket", "Protocol not available",            \
        "Protocol not supported", "Socket type not supported",                 \
        "Operation not supported", "Protocol family not supported",            \
        "Address family not supported by protocol", "Address already in use",  \
        "Cannot assign requested address", "Network is down",                  \
        "Network is unreachable", "Network dropped connection on reset",       \
        "Software caused connection abort", "Connection reset by peer",        \
        "No buffer space available",                                           \
        "Transport endpoint is already connected",                             \
        "Transport endpoint is not connected",                                 \
        "Cannot send after transport endpoint shutdown",                       \
        "Too many references: cannot splice", "Connection timed out",          \
        "Connection refused", "Host is down", "No route to host",              \
        "Operation already in progress", "Operation now in progress",          \
        "Stale file handle", "Structure needs cleaning",                       \
        "Not a XENIX named type file", "No XENIX semaphores available",        \
        "Is a named type file", "Remote I/O error", "Disk quota exceeded",     \
        "No medium found", "Wrong medium type", "Operation canceled",          \
        "Required key not available", "Key has expired",                       \
        "Key has been revoked", "Key was rejected by service", "Owner died",   \
        "State not recoverable", "Operation not possible due to RF-kill",      \
        "Memory page has hardware error"                                       \
  }
/*further down the list: "Unknown error: 134" etc.*/
#define MAX_ERROR_INDEX 133
#define ERR_MESS_PATTERN "Unknown error "
#endif

/*string.h Функции*/

void *s21_memchr(const void *str, int c, s21_size_t n);
// Выполняет поиск первого вхождения символа c(беззнаковый тип) в первых n
// байтах строки, на которую указывает аргумент str.

int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
// Сравнивает первые n байтов str1 и str2.

void *s21_memcpy(void *dest, const void *src, s21_size_t n);
// Копирует n символов из src в dest.

void *s21_memmove(void *dest, const void *src, s21_size_t n);
// Еще одна функция для копирования n символов из src в dest.

void *s21_memset(void *str, int c, s21_size_t n);
// Копирует символ c(беззнаковый тип) в первые n символов строки, на которую
// указывает аргумент str.

char *s21_strcat(char *dest, const char *src);
// Добавляет строку, на которую указывает src, в конец строки, на которую
// указывает dest.

char *s21_strncat(char *dest, const char *src, s21_size_t n);
// Добавляет строку, на которую указывает src, в конец строки, на которую
// указывает dest, длиной до n символов.

char *s21_strchr(const char *str, int c);
// Выполняет поиск первого вхождения символа c(беззнаковый тип) в строке, на
// которую указывает аргумент str.

int s21_strcmp(const char *str1, const char *str2);
// Сравнивает строку, на которую указывает str1, со строкой, на которую
// указывает str2.

int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
// Сравнивает не более первых n байтов str1 и str2.

char *s21_strcpy(char *dest, const char *src);
// Копирует строку, на которую указывает src, в dest.

char *s21_strncpy(char *dest, const char *src, s21_size_t n);
// Копирует до n символов из строки, на которую указывает src, в dest.

s21_size_t s21_strcspn(const char *str1, const char *str2);
// Вычисляет длину начального сегмента str1, который полностью состоит из
// символов, не входящих в str2.

char *s21_strerror(int errnum);
// Выполняет поиск во внутреннем массиве номера ошибки errnum и возвращает
// указатель на строку с сообщением об ошибке.Нужно объявить макросы, содержащие
// массивы сообщений об ошибке для операционных систем mac и linux. Описания
// ошибок есть в оригинальной библиотеке. Проверка текущей ОС осуществляется с
// помощью директив.

s21_size_t s21_strlen(const char *str);
// Вычисляет длину строки str, не включая завершающий нулевой символ.

char *s21_strpbrk(const char *str1, const char *str2);
// Находит первый символ в строке str1, который соответствует любому символу,
// указанному в str2.

char *s21_strrchr(const char *str, int c);
// Выполняет поиск последнего вхождения символа c(беззнаковый тип) в строке, на
// которую указывает аргумент str.

s21_size_t s21_strspn(const char *str1, const char *str2);
// Вычисляет длину начального сегмента str1, который полностью состоит из
// символов str2.

char *s21_strstr(const char *haystack, const char *needle);
// Находит первое вхождение всей строки needle(не включая завершающий нулевой
// символ), которая появляется в строке haystack.

char *s21_strtok(char *str, const char *delim);
// Разбивает строку str на ряд токенов, разделенных delim.

/*sprintf and sscanf*/

int s21_sscanf(const char *str, const char *format, ...);
// считывает форматированный ввод из строки.

int s21_sprintf(char *str, const char *format, ...);
// отправляет форматированный вывод в строку, на которую указывает str.

/*Специальные функции обработки строк (вдохновленные классом String в языке
 * C#)*/

void *s21_to_upper(const char *str);
// Возвращает копию строки(str), преобразованной в верхний регистр.В случае
// какой - либо ошибки следует вернуть значение NULL

void *s21_to_lower(const char *str);
// Возвращает копию строки(str), преобразованной в нижний регистр.В случае какой
// - либо ошибки следует вернуть значение NULL

void *s21_insert(const char *src, const char *str, s21_size_t start_index);
// Возвращает новую строку, в которой указанная строка(str) вставлена в
// указанную позицию(start_index) в данной строке(src).В случае какой - либо
// ошибки следует вернуть значение NULL

void *s21_trim(const char *src, const char *trim_chars);
// Возвращает новую строку, в которой удаляются все начальные и конечные
// вхождения набора заданных символов(trim_chars) из данной строки(src).В случае
// какой - либо ошибки следует вернуть значение NULL

#endif  // C2_S21_STRINGPLUS_S21_STRING_H_
