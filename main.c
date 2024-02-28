#include "./linked-list/linked-list.h"
#include "./ring-buffer/ring-buffer.h"
#include "./stack/stack.h"
#include <stdio.h>

int main(void) {

  LinkedList *ll = ll_create();

  ll_add(ll, (ListValueIn){.type = LL_INTEGER, .int_val = 10});
  ll_add(ll, (ListValueIn){.type = LL_STRING, .string = "hello, "});
  ll_add(ll, (ListValueIn){.type = LL_INTEGER, .int_val = 20});
  ll_add(ll, (ListValueIn){.type = LL_STRING, .string = "world!"});

  ll_print(ll);
  printf("size: %zu\n", ll_size(ll));
  ll_destroy(ll);

  printf("\n");

  Stack *s = s_create();

  s_push(s, (StackValueIn){.type = S_INTEGER, .integer = 5});
  s_push(s, (StackValueIn){.type = S_STRING, .string = "world"});
  s_push(s, (StackValueIn){.type = S_INTEGER, .integer = 10});
  s_push(s, (StackValueIn){.type = S_STRING, .string = "hello"});

  s_print(s);
  StackValueOut out = s_pop(s);
  out = s_pop(s);
  out = s_pop(s);
  out = s_pop(s);
  printf("out: %d\n", out.integer);
  out = s_pop(s);
  if (out.error != NULL) {
    printf("out: %d\n", *out.error);
  }

  printf("size: %zu\n", s_size(s));
  s_destroy(s);

  RingBuffer *rb = rb_create(5, 1.5);

  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 16});
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 15});
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 14});
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 13});
  RBValueOut out_rb = rb_deque(rb);
  out_rb = rb_deque(rb);
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 12});
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 11});

  rb_print(rb);
  printf("size: %zu\n", rb_length(rb));
  rb_destroy(rb);

  return 0;
}
