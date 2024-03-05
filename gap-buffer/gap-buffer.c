#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t length;
  size_t capacity;
  size_t gap_start;
  size_t gap_end;
  char *data;
} GapBuffer;

GapBuffer *gap_buffer_create(size_t initial_capacity) {
  GapBuffer *gb = malloc(sizeof(GapBuffer));
  if (!gb) {
    return NULL;
  }
  gb->data = malloc(sizeof(char) * initial_capacity);
  if (!gb->data) {
    free(gb);
    return NULL;
  }
  gb->length = 0;
  gb->capacity = initial_capacity;
  gb->gap_start = 0;
  gb->gap_end = initial_capacity;

  return gb;
}

void gap_buffer_destroy(GapBuffer *gb) {
  free(gb->data);
  free(gb);
}

size_t gap_buffer_length(GapBuffer *gb) { return gb->length; }

size_t gap_buffer_gap_size(GapBuffer *gb) {
  return gb->gap_end - gb->gap_start;
}

bool gap_buffer_grow(GapBuffer *gb) {
  size_t old_capacity = gb->capacity;
  gb->capacity *= 2;
  char *new_data = malloc(sizeof(char) * gb->capacity);
  if (!new_data) {
    return false;
  }

  memmove(new_data, gb->data, gb->gap_start);
  if (gb->gap_start != old_capacity) {
    memmove(new_data + old_capacity + gb->gap_start, gb->data + gb->gap_end,
            old_capacity - gb->gap_start);
  }

  free(gb->data);
  gb->data = new_data;

  return true;
}

bool gap_buffer_insert(GapBuffer *gb, char *value, size_t value_length) {
  size_t gap_size = gap_buffer_gap_size(gb);
  while (gap_size < value_length) {
    if (!gap_buffer_grow(gb)) {
      return false;
    }
    gap_size = gap_buffer_gap_size(gb);
  }

  memcpy(gb->data + gb->gap_start, value, value_length);
  gb->gap_start += value_length;
  gb->length += value_length;

  return true;
}

void gap_buffer_delete_left(GapBuffer *gb, size_t amount) {
  if (gb->gap_start < amount) {
    amount = gb->gap_start;
  }

  gb->gap_start -= amount;
  gb->length -= amount;
}

void gap_buffer_delete_right(GapBuffer *gb, size_t amount) {
  if (gb->capacity - gb->gap_end < amount) {
    amount = gb->capacity - gb->gap_end;
  }
  printf("gap_end: %zu, cap: %zu\n", gb->gap_end, gb->capacity);

  gb->gap_end += amount;
  gb->length -= amount;
}

void gap_buffer_left(GapBuffer *gb, size_t amount) {
  if (gb->gap_start < amount) {
    amount = gb->gap_start;
  }

  memmove(gb->data + gb->gap_end - amount, gb->data + gb->gap_start - amount,
          amount);
  gb->gap_start -= amount;
  gb->gap_end -= amount;
}

void gap_buffer_right(GapBuffer *gb, size_t amount) {
  if (gb->capacity - gb->gap_end < amount) {
    amount = gb->capacity - gb->gap_end;
  }

  memmove(gb->data + gb->gap_start, gb->data + gb->gap_end, amount);
  gb->gap_start += amount;
  gb->gap_end += amount;
}

void gap_buffer_str(GapBuffer *gb, char *str, size_t str_length) {
  if (gb->length == 0) {
    return;
  }
  if (str_length > gb->length) {
    str_length = gb->length;
  }

  if (str_length > gb->gap_start) {
    memcpy(str, gb->data, gb->gap_start);
    memcpy(str + gb->gap_start, gb->data + gb->gap_end,
           str_length - gb->gap_start);
  } else {
    memcpy(str, gb->data, str_length);
  }
  str[str_length] = '\0';
}
