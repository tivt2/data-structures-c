#ifndef HEAP
#define HEAP

#include <stdbool.h>
#include <stddef.h>

typedef enum { EMPTY_HEAP } HeapError;

typedef enum { HP_INTEGER, HP_STRING } HeapValueType;

typedef struct {
  HeapValueType type;
  union {
    int integer;
    char *string;
  };
} HeapValueIn;

typedef struct {
  const HeapError *error;
  HeapValueType type;
  union {
    int integer;
    char *string;
  };
} HeapValueOut;

typedef bool (*HeapCmpFn)(HeapValueIn, HeapValueIn);

typedef struct Heap Heap;

Heap *hp_create(size_t initial_size, HeapCmpFn cmp_fn);

void hp_destroy(Heap *hp);

size_t hp_length(Heap *hp);

bool hp_empty(Heap *hp);

void hp_push(Heap *hp, HeapValueIn value);

HeapValueOut hp_pop(Heap *hp);

void hp_print(Heap *hp);

#endif // !HEAP
