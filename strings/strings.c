#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Chunk {
  size_t capacity;
  char raw[];
} Chunk;

typedef struct {
  size_t length;
  union {
    struct Chunk *data;
    const char *raw;
  };
  bool is_mutable;
} String;

String *string(const char *raw, bool is_mutable) {
  size_t length = strlen(raw);
  size_t capacity = length * 2;

  String *string;
  if (is_mutable) {
    string = malloc(sizeof(String));
    if (!string) {
      return NULL;
    }
    string->data = malloc(sizeof(Chunk) + capacity * sizeof(char));
    if (!string->data) {
      free(string);
      return NULL;
    }
    string->is_mutable = true;
    string->data->capacity = capacity;
    memcpy(string->data->raw, raw, length);
  } else {
    string->is_mutable = false;
    string->raw = raw;
  }
  string->length = length;

  return string;
}

void string_destroy(String *str) {
  if (str->is_mutable) {
    free(str->data);
  }
  free(str);
}

size_t s_length(String *str) { return str->length; }

String *s_slice(String *str, size_t start, size_t end) {
  size_t length = end - start;
  char *slice = malloc((length + 1) * sizeof(char));
  if (!slice) {
    return NULL;
  }
  if (str->is_mutable) {
    memcpy(slice, str->data->raw + start, length);
  } else {
    memcpy(slice, str->raw + start, length);
  }
  slice[length] = '\0';

  String *out = string(slice, str->is_mutable);
  free(slice);
  return out;
}

String *s_append(String *str1, const char *str2) {
  size_t str2_len = strlen(str2);
  char *new_str = malloc((str1->length + str2_len + 1) * sizeof(char));
  if (!new_str) {
    return NULL;
  }
  if (str1->is_mutable) {
    memcpy(new_str, str1->data->raw, str1->length);
  } else {
    memcpy(new_str, str1->raw, str1->length);
  }
  memcpy(new_str + str1->length, str2, str2_len);
  new_str[str1->length + str2_len] = '\0';

  String *out = string(new_str, str1->is_mutable);
  free(new_str);
  return out;
}

String *s_concat(String *str1, String *str2) {
  char *new_str = malloc(sizeof(char) * (str1->length + str2->length + 1));
  if (!new_str) {
    return NULL;
  }

  if (str1->is_mutable) {
    memcpy(new_str, str1->data->raw, str1->length);
  } else {
    memcpy(new_str, str1->raw, str1->length);
  }
  if (str2->is_mutable) {

    memcpy(new_str + str1->length, str2->data->raw, str2->length);
  } else {
    memcpy(new_str + str1->length, str2->raw, str2->length);
  }

  new_str[str1->length + str2->length] = '\0';

  String *out = string(new_str, str1->is_mutable);
  free(new_str);
  return out;
}

void s_out(char *dest, String *str) {
  if (str->is_mutable) {
    memcpy(dest, str->data->raw, str->length);
  } else {
    memcpy(dest, str->raw, str->length);
  }

  dest[str->length] = '\0';
}
