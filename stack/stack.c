#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_ERRORS_AMOUNT 1

typedef enum { EMPTY_STACK } StackError;

const StackError global_stack_errors[STACK_ERRORS_AMOUNT];

typedef enum { S_INTEGER, S_STRING } StackValueType;

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

typedef struct StackNode {
  StackValueIn value;
  struct StackNode *next;
} StackNode;

typedef struct Stack {
  size_t size;
  StackNode *tail;
} Stack;

Stack *s_create(void) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  if (!s) {
    return NULL;
  }
  s->size = 0;
  s->tail = NULL;

  return s;
}

void s_destroy(Stack *stack) {
  StackNode *curr = stack->tail;
  while (curr) {
    StackNode *next = curr->next;
    free(curr);
    curr = next;
  }
  free(stack);
}

size_t s_size(Stack *stack) { return stack->size; }

bool s_empty(Stack *stack) { return s_size(stack) > 0; }

bool s_push(Stack *stack, StackValueIn value) {
  StackNode *node = (StackNode *)malloc(sizeof(StackNode));
  if (!node) {
    return false;
  }
  node->value = value;
  node->next = NULL;

  if (!stack->tail) {
    stack->tail = node;
  } else {
    node->next = stack->tail;
    stack->tail = node;
  }

  stack->size++;
  return true;
}

StackValueOut s_pop(Stack *stack) {
  StackValueOut out;
  if (stack->size == 0) {
    out.error = &global_stack_errors[EMPTY_STACK];
    return out;
  }
  out.error = NULL;

  StackNode *node = stack->tail;
  if (!node->next) {
    stack->tail = NULL;
  } else {
    stack->tail = node->next;
  }

  out.type = node->value.type;
  switch (out.type) {
  case S_INTEGER:
    out.integer = node->value.integer;
    break;
  case S_STRING:
    out.string = node->value.string;
    break;
  }

  free(node);

  stack->size--;
  return out;
}

void s_print(Stack *stack) {
  StackNode *curr = stack->tail;
  printf("[");
  while (curr) {
    if (curr != stack->tail) {
      printf(", ");
    }
    switch (curr->value.type) {
    case S_INTEGER:
      printf("%d", curr->value.integer);
      break;
    case S_STRING:
      printf("%s", curr->value.string);
      break;
    }
    curr = curr->next;
  }
  printf("]\n");
}
