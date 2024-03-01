#include "binary-tree/binary-tree.h"
#include "heap/heap.h"
#include "linked-list/linked-list.h"
#include "ring-buffer/ring-buffer.h"
#include "stack/stack.h"
#include <stdio.h>

bool heap_cmp_fn(HeapValueIn val1, HeapValueIn val2) {
  return val2.integer < val1.integer;
}

int binary_tree_cmp_fn(BinaryTreeValue val1, BinaryTreeValue val2) {
  if (val1.integer == val2.integer) {
    return 0;
  } else if (val1.integer < val2.integer) {
    return 1;
  }
  return -1;
}

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

  printf("\n");

  RingBuffer *rb = rb_create(3, 2);

  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 16});
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 15});
  printf("rb_deque: %d\n", rb_deque(rb).integer);
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 14});
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 13});
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 12});
  rb_print(rb);
  printf("rb_deque: %d\n", rb_deque(rb).integer);
  rb_enqueue(rb, (RBValueIn){.type = RB_INTEGER, .integer = 11});

  rb_print(rb);
  printf("length: %zu\n", rb_length(rb));
  rb_destroy(rb);

  printf("\n");

  Heap *hp = hp_create(4, heap_cmp_fn);

  hp_push(hp, (HeapValueIn){.type = HP_INTEGER, .integer = 10});
  hp_push(hp, (HeapValueIn){.type = HP_INTEGER, .integer = 11});
  hp_push(hp, (HeapValueIn){.type = HP_INTEGER, .integer = 4});
  hp_push(hp, (HeapValueIn){.type = HP_INTEGER, .integer = 5});
  hp_push(hp, (HeapValueIn){.type = HP_INTEGER, .integer = 1});
  hp_print(hp);
  printf("hp_pop: %d\n", hp_pop(hp).integer);
  printf("hp_pop: %d\n", hp_pop(hp).integer);

  hp_print(hp);
  hp_destroy(hp);

  printf("\n");

  BinaryTree *bt = bt_create(binary_tree_cmp_fn);

  bt_insert(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 10});
  bt_insert(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 8});
  bt_insert(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 16});
  bt_insert(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 20});
  bt_insert(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 17});
  bt_insert(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 18});
  bt_insert(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 22});
  bt_print(bt);
  printf("\n");
  bt_DFS_delete(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 16});
  bt_print(bt);
  printf("\n");
  bt_DFS_delete(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 10});
  bt_print(bt);
  printf("\n");
  bt_DFS_delete(bt, (BinaryTreeValue){.type = BT_INTEGER, .integer = 18});
  bt_print(bt);

  bt_destroy(bt);
  printf("\n");

  return 0;
}
