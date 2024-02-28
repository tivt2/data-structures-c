#include "./linked-list/linked-list.h"
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

  return 0;
}
