#ifndef GAP_BUFFER
#define GAP_BUFFER

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct GapBuffer GapBuffer;

GapBuffer *gap_buffer_create(size_t initial_capacity);

void gap_buffer_destroy(GapBuffer *gb);

size_t gap_buffer_length(GapBuffer *gb);

bool gap_buffer_insert(GapBuffer *gb, char *value, size_t value_length);

void gap_buffer_delete_left(GapBuffer *gb, size_t amount);

void gap_buffer_delete_right(GapBuffer *gb, size_t amount);

void gap_buffer_left(GapBuffer *gb, size_t amount);

void gap_buffer_right(GapBuffer *gb, size_t amount);

void gap_buffer_str(GapBuffer *gb, char *str, size_t str_length);

#endif // !GAP_BUFFER
