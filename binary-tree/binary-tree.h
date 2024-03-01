#ifndef BINARY_TREE
#define BINARY_TREE

#include <stdbool.h>
#include <stddef.h>

typedef enum { EMPTY_BINARY_TREE } BinaryTreeErrors;

typedef enum { BT_INTEGER, BT_STRING } BinaryTreeValueType;

typedef struct {
  BinaryTreeValueType type;
  union {
    int integer;
    char *string;
  };
} BinaryTreeValue;

typedef int (*BinaryTreeCmpFn)(BinaryTreeValue, BinaryTreeValue);

typedef struct BinaryTree BinaryTree;

BinaryTree *bt_create(BinaryTreeCmpFn cmp_fn);

void bt_destroy(BinaryTree *bt);

size_t bt_length(BinaryTree *bt);

bool bt_empty(BinaryTree *bt);

void bt_insert(BinaryTree *bt, BinaryTreeValue value);

bool bt_DFS(BinaryTree *bt, BinaryTreeValue value);

bool bt_BFS(BinaryTree *bt, BinaryTreeValue value);

bool bt_DFS_delete(BinaryTree *bt, BinaryTreeValue value);

bool bt_BFS_delete(BinaryTree *bt, BinaryTreeValue value);

void bt_print(BinaryTree *bt);

#endif // !BINARY_TREE
