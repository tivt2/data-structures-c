#include "binary-tree/binary-tree.h"
#include "gap-buffer/gap-buffer.h"
#include "hashtable/hashtable.h"
#include "heap/heap.h"
#include "linked-list/linked-list.h"
#include "ring-buffer/ring-buffer.h"
#include "stack/stack.h"
#include "strings/strings.h"
#include "trie/trie.h"
#include <stdio.h>
#include <stdlib.h>

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

  String *s1 = string("Foo", true);
  String *s2 = string("Bar", true);

  String *s3 = s_concat(s1, s2);
  char *s3_raw = malloc(s_length(s3) + 1);
  s_out(s3_raw, s3);
  printf("%s\n", s3_raw);
  free(s3_raw);

  String *s4 = s_append(s1, "Baz");
  char *s4_raw = malloc(s_length(s4) + 1);
  s_out(s4_raw, s4);
  printf("%s\n", s4_raw);
  free(s4_raw);

  String *s5 = s_slice(s4, 1, 5);
  char *s5_raw = malloc(s_length(s5) + 1);
  s_out(s5_raw, s5);
  printf("%s\n", s5_raw);
  free(s5_raw);

  string_destroy(s1);
  string_destroy(s2);
  string_destroy(s3);
  string_destroy(s4);
  string_destroy(s5);

  printf("\n");

  Trie *trie = trie_create();

  trie_insert(trie, "foo");
  trie_insert(trie, "bas");
  trie_insert(trie, "floor");
  trie_insert(trie, "baz");
  trie_print(trie);

  trie_delete(trie, "foo");
  trie_delete(trie, "baz");
  trie_print(trie);

  trie_destroy(trie);

  printf("\n");

  HashTable *ht = ht_create(1);

  ht_insert(ht, (HTEntry){.key = "foo", .value.integer = 0});
  ht_insert(ht, (HTEntry){.key = "baz", .value.integer = 1});
  ht_insert(ht, (HTEntry){.key = "bar", .value.integer = 2});
  ht_insert(ht, (HTEntry){.key = "AAA", .value.integer = 3});
  ht_print(ht);
  HTEntry foo = ht_get(ht, "foo");
  printf("ht_get(): [key: %s, val: %d]\n", foo.key, foo.value.integer);
  ht_delete(ht, "bar");
  ht_delete(ht, "AAA");
  ht_delete(ht, "foo");
  ht_print(ht);

  ht_destroy(ht);

  printf("\n");

  GapBuffer *buf = gap_buffer_create(32);

  gap_buffer_insert(buf, "hello", 5);
  gap_buffer_insert(buf, "l!", 2);

  char str[15];
  gap_buffer_str(buf, str, 15);
  printf("%s\n", str);

  gap_buffer_left(buf, 2);
  gap_buffer_insert(buf, " wor", 4);

  gap_buffer_str(buf, str, 15);
  printf("%s\n", str);

  gap_buffer_right(buf, 1);
  gap_buffer_insert(buf, "d", 1);

  gap_buffer_str(buf, str, 15);
  printf("%s\n", str);

  gap_buffer_delete_right(buf, 1);

  gap_buffer_str(buf, str, 15);
  printf("%s\n", str);

  gap_buffer_delete_left(buf, 4);

  gap_buffer_str(buf, str, 15);
  printf("%s\n", str);

  gap_buffer_destroy(buf);

  return 0;
}
