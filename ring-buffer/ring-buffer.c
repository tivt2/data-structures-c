#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  size_t growth;
  size_t size;

  size_t head;
  size_t tail;
  size_t len;

  RBValueIn *data;
} RingBuffer;

// growth must be > 1, growth of 1 represents no growth
RingBuffer *rb_create(size_t size, size_t growth) {
  RingBuffer *rb = malloc(sizeof(RingBuffer));
  if (!rb) {
    return NULL;
  }
  rb->head = 0;
  rb->tail = 0;
  rb->len = 0;
  rb->size = size;
  rb->growth = growth < 1 ? 1 : growth;
  rb->data = malloc(sizeof(RBValueIn) * size);
  if (!rb->data) {
    return NULL;
  }

  return rb;
}

size_t rb_length(RingBuffer *rb) { return rb->len; }

size_t rb_empty(RingBuffer *rb) { return rb->len == 0; }

void rb_destroy(RingBuffer *rb) {
  free(rb->data);
  free(rb);
}

void rb_grow(RingBuffer *rb) {
  if (rb->len < (rb->size)) {
    return;
  }

  rb->size *= rb->growth;
  rb->data = realloc(rb->data, rb->size * sizeof(RBValueIn));

  if (rb->head == 0) {
    rb->tail = rb->len;
    return;
  }

  size_t right_empty = rb->size - rb->head - 1;
  if (right_empty < (rb->tail)) {
    memcpy(&rb->data[rb->len], &rb->data[0], right_empty * sizeof(RBValueIn));
    memcpy(&rb->data[0], &rb->data[right_empty],
           (rb->tail - right_empty) * sizeof(RBValueIn));
  } else {
    memcpy(&rb->data[rb->len], &rb->data[0], rb->tail * sizeof(RBValueIn));
  }
  rb->tail = (rb->len + rb->head) % rb->size;
}

void rb_enqueue(RingBuffer *rb, RBValueIn value) {
  if (rb->growth > 1) {
    rb_grow(rb);
  } else if (rb->tail == rb->head) {
    rb->head = (rb->head + 1) % rb->size;
    rb->len--;
  }

  rb->data[rb->tail] = value;
  rb->tail = (rb->tail + 1) % rb->size;
  rb->len++;
}

RBValueOut rb_deque(RingBuffer *rb) {
  RBValueOut out;
  if (rb->len == 0) {
    out.error = &global_ring_buffer_errors[EMPTY_RING_BUFFER];
    return out;
  }

  RBValueIn node = rb->data[rb->head];

  out.type = node.type;
  switch (out.type) {
  case RB_INTEGER:
    out.integer = node.integer;
    break;
  case RB_STRING:
    out.string = node.string;
    break;
  }

  rb->head = (rb->head + 1) % rb->size;
  rb->len--;
  return out;
}

void rb_print(RingBuffer *rb) {
  printf("[");
  for (int i = 0; i < (rb->len); i++) {
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
