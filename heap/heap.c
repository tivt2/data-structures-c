#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_ERRORS_COUNT 1
typedef enum { EMPTY_HEAP } HeapError;

const HeapError global_heap_errors[HEAP_ERRORS_COUNT];

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

typedef struct {
  size_t size;
  size_t len;
  HeapCmpFn cmp_fn;
  HeapValueIn *data;
} Heap;

// initial_size is a multiple of 2, e.g: initial_size = 4; heap will have 2^4
// values spaces
Heap *hp_create(size_t initial_size, HeapCmpFn cmp_fn) {
  Heap *hp = malloc(sizeof(Heap));
  if (!hp) {
    return NULL;
  }

  hp->size = 1 << initial_size;
  hp->len = 0;
  hp->cmp_fn = cmp_fn;
  hp->data = malloc(sizeof(HeapValueIn) * hp->size);
  if (!hp->data) {
    return NULL;
  }

  return hp;
}

void hp_destroy(Heap *hp) {
  free(hp->data);
  free(hp);
}

size_t hp_length(Heap *hp) { return hp->len; }

bool hp_empty(Heap *hp) { return hp->len == 0; }

void hp_swap(Heap *hp, size_t idx1, size_t idx2) {
  HeapValueIn tmp = hp->data[idx1];
  hp->data[idx1] = hp->data[idx2];
  hp->data[idx2] = tmp;
}

void hp_heapfyup(Heap *hp, size_t idx) {
  if (idx <= 0) {
    return;
  }

  size_t parent_idx = (idx - 1) / 2;
  if (!hp->cmp_fn(hp->data[parent_idx], hp->data[idx])) {
    return;
  }

  hp_swap(hp, idx, parent_idx);

  hp_heapfyup(hp, parent_idx);
}

void hp_heapfydown(Heap *hp, size_t idx) {
  size_t chilren1_idx = idx * 2 + 1;
  size_t chilren2_idx = idx * 2 + 2;
  if (chilren1_idx >= (hp->len)) {
    return;
  }

  size_t swap_idx;
  if (chilren2_idx < (hp->len)) {
    if (hp->cmp_fn(hp->data[chilren1_idx], hp->data[chilren2_idx])) {
      swap_idx = chilren2_idx;
    } else {
      swap_idx = chilren1_idx;
    }
  } else {
    swap_idx = chilren1_idx;
  }

  if (!hp->cmp_fn(hp->data[idx], hp->data[swap_idx])) {
    return;
  }

  hp_swap(hp, swap_idx, idx);

  hp_heapfydown(hp, swap_idx);
}

void hp_grow(Heap *hp) {
  if (hp->len < (hp->size)) {
    return;
  }

  hp->size *= 2;
  hp->data = realloc(hp->data, hp->size * sizeof(HeapValueIn));
}

void hp_push(Heap *hp, HeapValueIn value) {
  // insert new value at last position them compares/swap with parent until no
  // more swaps needed.
  //
  hp_grow(hp);

  hp->data[hp->len] = value;
  hp_heapfyup(hp, hp->len);

  hp->len++;
}

HeapValueOut hp_pop(Heap *hp) {
  // swap last with first, compares its children than compares/swap the winner
  // with parent until no more return last element
  //
  HeapValueOut out;
  if (hp->len == 0) {
    out.error = &global_heap_errors[EMPTY_HEAP];
    return out;
  }
  out.error = NULL;

  hp->len--;
  hp_swap(hp, hp->len, 0);
  hp_heapfydown(hp, 0);

  HeapValueIn node = hp->data[hp->len];
  out.type = node.type;
  switch (out.type) {
  case HP_INTEGER:
    out.integer = node.integer;
    break;
  case HP_STRING:
    out.string = node.string;
    break;
  }

  return out;
}

void hp_print(Heap *hp) {
  printf("[");
  for (size_t i = 0; i < (hp->len); i++) {
    if (i != 0) {
      printf(", ");
    }
    HeapValueIn node = hp->data[i];
    switch (node.type) {
    case HP_INTEGER:
      printf("%d", node.integer);
      break;
    case HP_STRING:
      printf("%s", node.string);
      break;
    }
  }
  printf("]\n");
}
