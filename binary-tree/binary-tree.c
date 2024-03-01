#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define BINARY_TREE_ERRORS_COUNT 2
typedef enum { EMPTY_BINARY_TREE, VALUE_NOT_FOUND } BinaryTreeErrors;

const BinaryTreeErrors global_binary_tree_erros[BINARY_TREE_ERRORS_COUNT];

typedef enum { T_INTEGER, T_STRING } BinaryTreeValueType;

typedef struct {
  BinaryTreeValueType type;
  union {
    int integer;
    char *string;
  };
} BinaryTreeValueIn;

typedef struct BinaryTreeNode {
  BinaryTreeValueIn value;
  struct BinaryTreeNode *left;
  struct BinaryTreeNode *right;
} BinaryTreeNode;

// 1 if right value wins, -1 if left value wins, 0 if equal(used in search)
typedef int (*BinaryTreeCmpFn)(BinaryTreeValueIn, BinaryTreeValueIn);

typedef struct {
  BinaryTreeNode *root;
  size_t len;
  BinaryTreeCmpFn cmp_fn;
} BinaryTree;

BinaryTree *bt_create(BinaryTreeCmpFn cmp_fn) {
  BinaryTree *bt = malloc(sizeof(BinaryTree));
  if (!bt) {
    return NULL;
  }
  bt->len = 0;
  bt->cmp_fn = cmp_fn;
  bt->root = NULL;

  return bt;
}

void node_destroy(BinaryTreeNode *node) {
  if (!node) {
    return;
  }

  BinaryTreeNode *left = node->left;
  BinaryTreeNode *right = node->right;
  free(node);
  if (!!left) {
    node_destroy(left);
  }
  if (!!right) {
    node_destroy(right);
  }
}

void bt_destroy(BinaryTree *bt) {
  node_destroy(bt->root);
  free(bt);
}

size_t bt_length(BinaryTree *bt) { return bt->len; }

bool bt_empty(BinaryTree *bt) { return bt->len == 0; }

void bt_insert(BinaryTree *bt, BinaryTreeValueIn value) {

  BinaryTreeNode *parent = NULL;
  BinaryTreeNode *curr = bt->root;
  while (curr) {
    parent = curr;
    if (bt->cmp_fn(curr->value, value) == 1) {
      curr = curr->right;
    } else {
      curr = curr->left;
    }
  }

  BinaryTreeNode *node = malloc(sizeof(BinaryTreeNode));
  curr->left = NULL;
  curr->right = NULL;
  curr->value = value;

  if (!parent) {
    bt->root = node;
  } else if (bt->cmp_fn(parent->value, value)) {
    parent->right = node;
  } else {
    parent->left = node;
  }

  bt->len++;
}

bool bt_DFS(BinaryTree *bt, BinaryTreeValueIn value) {
  if (!bt->root) {
    return false;
  }

  BinaryTreeNode *to_visit[bt->len];
  to_visit[0] = bt->root;
  size_t pos = 1;
  while (pos != 0) {
    BinaryTreeNode *curr = to_visit[--pos];
    if (bt->cmp_fn(curr->value, value)) {
      return true;
    }
    to_visit[pos++] = curr->right;
    to_visit[pos++] = curr->left;
  }

  return false;
}

bool bt_BFS(BinaryTree *bt, BinaryTreeValueIn value) {
  if (!bt->root) {
    return false;
  }

  BinaryTreeNode *to_visit[bt->len];
  to_visit[0] = bt->root;
  size_t head = 0;
  size_t tail = 1;
  while (head < tail) {
    BinaryTreeNode *curr = to_visit[head++];
    if (bt->cmp_fn(curr->value, value)) {
      return true;
    }
    to_visit[tail++] = curr->left;
    to_visit[tail++] = curr->right;
  }

  return false;
}

enum Dir { LEFT, RIGHT };
BinaryTreeNode *node_delete(BinaryTreeNode *parent, BinaryTreeNode *node,
                            enum Dir dir) {
  if (parent == NULL) {
  }
}

bool bt_DFS_delete(BinaryTree *bt, BinaryTreeValueIn value) {
  if (!bt->root) {
    return false;
  }

  BinaryTreeNode *to_visit[bt->len];
  to_visit[0] = bt->root;
  size_t pos = 1;
  size_t len = 1;
  while (pos != 0) {
    BinaryTreeNode *curr = to_visit[--pos];
    if (pos == 0) {
      BinaryTreeNode *node = node_delete(NULL, curr, RIGHT);
    } else if (bt->cmp_fn(curr->value, value)) {
      if (pos % 2 == 0) {
        node_delete(to_visit[(pos - 1) / 2], curr, RIGHT);
      }
      return true;
    }
    if (curr->right) {
      to_visit[len++] = curr->right;
      pos++;
    }
    if (curr->left) {
      to_visit[len++] = curr->left;
      pos++;
    }
    if (!curr->left && !curr->right) {
      pos++;
    }
  }

  return false;
}

bool bt_BFS_delete(BinaryTree *bt, BinaryTreeValueIn value) {}
