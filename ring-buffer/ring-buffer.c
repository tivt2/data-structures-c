#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define RING_BUFFER_ERROR_COUNT 1
typedef enum { EMPTY_RING_BUFFER } RBError;

const RBError global_ring_buffer_errors[RING_BUFFER_ERROR_COUNT];

typedef enum { RB_INTEGER, RB_STRING } RBValueType;

typedef struct {
  RBValueType type;
  union {
    int integer;
    char *string;
  };
} RBValueIn;

typedef struct {
  const RBError *error;
  RBValueType type;
  union {
    int integer;
    char *string;
  };
} RBValueOut;

typedef struct RingBuffer {
  float_t growth;
  size_t size;

  size_t head;
  size_t tail;

  RBValueIn *data;
} RingBuffer;

// growth must be > 1, growth of 1 represents no growth
RingBuffer *rb_create(size_t size, float_t growth) {
  RingBuffer *rb = malloc(sizeof(RingBuffer));
  if (!rb) {
    return NULL;
  }
  rb->head = 0;
  rb->tail = 0;
  rb->size = size;
  rb->growth = growth;
  rb->data = malloc(sizeof(RBValueIn) * size);

  return rb;
}

size_t rb_length(RingBuffer *rb) {
  return (rb->tail - rb->head + rb->size) % rb->size;
}

size_t rb_empty(RingBuffer *rb) { return rb_length(rb) == 0; }

void rb_destroy(RingBuffer *rb) {
  free(rb->data);
  free(rb);
}

void rb_grow(RingBuffer *rb) {
  if (rb->growth <= 1 || rb_length(rb) + 1 < rb->size) {
    return;
  }

  rb->size *= rb->growth;
  rb->data = realloc(rb->data, rb->size);

  // get the first element of data(data[0] corresponds to len - head)
  // from i = elementIDX until head, data[len+i % new_size] = element
  // new tail = len + head;
  // this only happens if head != 0
  if (rb->head == 0) {
    return;
  }

  size_t len = rb_length(rb);
  for (int i = 0; i < (rb->head); i++) {
    rb->data[(len + i) % rb->size] = rb->data[i];
  }

  rb->tail = (len + rb->head) % rb->size;
}

bool rb_enqueue(RingBuffer *rb, RBValueIn value) {
  rb_grow(rb);

  if ((rb->tail + 1) % rb->size == rb->head) {
    rb->head = (rb->head + 1) % rb->size;
  }
  rb->data[rb->tail] = value;
  rb->tail = (rb->tail + 1) % rb->size;

  return true;
}

RBValueOut rb_deque(RingBuffer *rb) {
  RBValueOut out;
  if (rb_length(rb) == 0) {
    out.error = &global_ring_buffer_errors[EMPTY_RING_BUFFER];
    return out;
  }

  RBValueIn node = rb->data[rb->head];
  rb->head = (rb->head + 1) % rb->size;

  out.type = node.type;
  switch (out.type) {
  case RB_INTEGER:
    out.integer = node.integer;
    break;
  case RB_STRING:
    out.string = node.string;
    break;
  }

  return out;
}

void rb_print(RingBuffer *rb) {
  printf("[");
  for (int i = 0; i < rb_length(rb); i++) {
    if (i != 0) {
      printf(", ");
    }
    RBValueIn node = rb->data[(i + rb->head) % rb->size];
    switch (node.type) {
    case RB_INTEGER:
      printf("%d", node.integer);
      break;
    case RB_STRING:
      printf("%s", node.string);
      break;
    }
  }
  printf("]\n");
}
