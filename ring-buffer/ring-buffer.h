#ifndef RING_BUFFER
#define RING_BUFFER

#include <stdbool.h>
#include <stddef.h>

typedef enum { EMPTY_RING_BUFFER } RBError;

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

typedef struct RingBuffer RingBuffer;

RingBuffer *rb_create(size_t size, size_t should_grow);

void rb_destroy(RingBuffer *rb);

size_t rb_length(RingBuffer *rb);

bool rb_empty(RingBuffer *rb);

void rb_enqueue(RingBuffer *rb, RBValueIn value);

RBValueOut rb_deque(RingBuffer *rb);

void rb_print(RingBuffer *rb);

#endif // !RING_BUFFER
