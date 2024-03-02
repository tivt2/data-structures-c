#ifndef STRINGS
#define STRINGS

#include <stdbool.h>
#include <stddef.h>

typedef struct String String;

String *string(const char *str, bool is_mutable);

void string_destroy(String *str);

size_t s_length(String *str);

String *s_slice(String *str, size_t start, size_t end);

String *s_append(String *str1, const char *str2);

String *s_concat(String *str1, String *str2);

void s_out(char *dest, String *str);

#endif // !STRINGS
