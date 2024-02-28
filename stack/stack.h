#ifndef STACK
#define STACK

#include <stddef.h>
#include <stdbool.h>

typedef enum { EMPTY_STACK } StackError;

typedef enum {S_INTEGER, S_STRING} StackValueType ;

typedef struct {
  StackValueType type;
  union {
    int integer;
    char *string;
  };
} StackValueIn;

typedef struct {
  const StackError *error;
  StackValueType type;
  union {
    int integer;
    char *string;
  };
} StackValueOut;

typedef struct Stack Stack;

Stack *s_create(void);

void s_destroy(Stack *stack);

size_t s_size(Stack *stack);

bool s_empty(Stack *stack);

bool s_push(Stack *stack, StackValueIn value);

StackValueOut s_pop(Stack *stack);

void s_print(Stack *stack);

#endif // !STACK
