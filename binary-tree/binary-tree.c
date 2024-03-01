#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BINARY_TREE_ERRORS_COUNT 2
typedef enum { EMPTY_BINARY_TREE, VALUE_NOT_FOUND } BinaryTreeErrors;

const BinaryTreeErrors global_binary_tree_erros[BINARY_TREE_ERRORS_COUNT];

typedef enum { BT_INTEGER, BT_STRING } BinaryTreeValueType;

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
  node->left = NULL;
  node->right = NULL;
  node->value = value;

  if (!parent) {
    bt->root = node;
  } else if (bt->cmp_fn(parent->value, value) == 1) {
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
    if (bt->cmp_fn(curr->value, value) == 0) {
      return true;
    }
    if (curr->right) {
      to_visit[pos++] = curr->right;
    }
    if (curr->left) {
      to_visit[pos++] = curr->left;
    }
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
    if (bt->cmp_fn(curr->value, value) == 0) {
      return true;
    }
    if (curr->left) {
      to_visit[tail++] = curr->left;
    }
    if (curr->right) {
      to_visit[tail++] = curr->right;
    }
  }

  return false;
}

BinaryTreeNode *node_extract_smallest(BinaryTreeNode *parent,
                                      BinaryTreeNode *node) {
  if (!node->left) {
    if (!!node->right) {
      parent->left = node->right;
      node->right = NULL;
    }
    return node;
  }

  return node_extract_smallest(node, node->left);
}

void bt_node_delete(BinaryTree *bt, BinaryTreeNode *parent,
                    BinaryTreeNode *node) {
  int dir;
  BinaryTreeNode *swap_node;
  if (parent == NULL) {
    if (!node->right) {
      bt->root = node->left;
    } else if (!node->right->left) {
      swap_node = node->right;
      swap_node->left = bt->root->left;
      bt->root = swap_node;
    } else {
      swap_node = node_extract_smallest(node, node->right);
      swap_node->right = bt->root->right;
      bt->root = swap_node;
    }
    free(node);
    return;
  }

  if (parent->left == node) {
    dir = -1;
  } else if (parent->right == node) {
    dir = 1;
  }

  if (!node->right) {
    swap_node = node->left;
  } else if (!node->right->left) {
    swap_node = node->right;
    swap_node->left = parent->left;
  } else {
    swap_node = node_extract_smallest(node, node->right);
    swap_node->right = node->right;
  }

  if (dir == -1) {
    parent->left = swap_node;
  } else if (dir == 1) {
    parent->right = swap_node;
  }
  free(node);
}

bool bt_DFS_delete(BinaryTree *bt, BinaryTreeValueIn value) {
  if (!bt->root) {
    return false;
  }

  BinaryTreeNode *to_visit[bt->len][2];
  to_visit[0][0] = NULL;
  to_visit[0][1] = bt->root;
  size_t pos = 1;
  while (pos != 0) {
    BinaryTreeNode *curr = to_visit[--pos][1];
    if (bt->cmp_fn(curr->value, value) == 0) {
      bt_node_delete(bt, to_visit[pos][0], curr);
      bt->len--;
      return true;
    }

    if (curr->right) {
      to_visit[pos][0] = curr;
      to_visit[pos++][1] = curr->right;
    }
    if (curr->left) {
      to_visit[pos][0] = curr;
      to_visit[pos++][1] = curr->left;
    }
  }

  return false;
}

bool bt_BFS_delete(BinaryTree *bt, BinaryTreeValueIn value) {
  if (!bt->root) {
    return false;
  }

  BinaryTreeNode *to_visit[bt->len][2];
  to_visit[0][0] = NULL;
  to_visit[0][1] = bt->root;
  size_t head = 0;
  size_t tail = 1;
  while (head < tail) {
    BinaryTreeNode *parent = to_visit[head][0];
    BinaryTreeNode *curr = to_visit[head++][1];
    if (bt->cmp_fn(curr->value, value) == 0) {
      bt_node_delete(bt, parent, curr);
      bt->len--;
      return true;
    }
    if (curr->left) {
      to_visit[tail][0] = curr;
      to_visit[tail++][1] = curr->left;
    }
    if (curr->right) {
      to_visit[tail][0] = curr;
      to_visit[tail++][1] = curr->right;
    }
  }

  return false;
}

void bt_print(BinaryTree *bt) {
  if (!bt->root) {
    printf("[]");
    return;
  }

  BinaryTreeNode *to_visit[1 << bt->len];
  to_visit[0] = bt->root;
  size_t head = 0;
  size_t tail = 1;
  size_t height = 0;

  size_t to_print = bt->len;

  while (to_print > 0) {
    size_t nodes_processed = 0;

    while (nodes_processed < (1 << height)) {
      BinaryTreeNode *curr = to_visit[head++];
      nodes_processed++;

      if (!!curr) {
        printf("%d ", curr->value.integer);
        to_print--;

        to_visit[tail++] = curr->left;
        to_visit[tail++] = curr->right;

      } else {
        printf("-1 ");
        to_visit[tail++] = NULL;
        to_visit[tail++] = NULL;
      }
    }

    printf("\n");
    height++;
  }
}
