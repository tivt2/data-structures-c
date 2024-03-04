#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_CHARS 256

typedef struct TrieNode {
  struct TrieNode *children[NUM_CHARS];
  bool terminal;
} TrieNode;

typedef TrieNode Trie;

TrieNode *trie_node_create() {
  TrieNode *node = malloc(sizeof(TrieNode));
  if (!node) {
    return NULL;
  }

  for (int i = 0; i < NUM_CHARS; i++) {
    node->children[i] = NULL;
  }

  node->terminal = false;

  return node;
}

Trie *trie_create() {
  Trie *trie = trie_node_create();
  if (!trie) {
    return NULL;
  }

  return trie;
}

void trie_destroy(Trie *trie) {
  for (int i = 0; i < NUM_CHARS; i++) {
    if (trie->children[i] != NULL) {
      trie_destroy(trie->children[i]);
    }
  }
  free(trie);
}

bool trie_insert(Trie *trie, char *signedtext) {
  unsigned char *text = (unsigned char *)signedtext;
  int length = strlen(signedtext);
  TrieNode *tmp = trie;

  for (int i = 0; i < length; i++) {
    unsigned char c = text[i];
    if (tmp->children[c] == NULL) {
      tmp->children[c] = trie_node_create();
    }
    tmp = tmp->children[c];
  }

  if (tmp->terminal) {
    return false;
  }

  tmp->terminal = true;
  return true;
}

bool trie_node_has_children(TrieNode *node) {
  if (node == NULL) {
    return false;
  }
  for (int i = 0; i < NUM_CHARS; i++) {
    if (node->children[i] != NULL) {
      return true;
    }
  }

  return false;
}

TrieNode *trie_node_delete(TrieNode *node, unsigned char *text, bool *deleted) {
  if (node == NULL) {
    return node;
  }

  if (*text == '\0') {
    if (node->terminal) {
      node->terminal = false;
      *deleted = true;

      if (trie_node_has_children(node) == false) {
        free(node);
        node = NULL;
      }
    }
    return node;
  }

  node->children[text[0]] =
      trie_node_delete(node->children[text[0]], text + 1, deleted);
  if (*deleted && trie_node_has_children(node) == false &&
      node->terminal == false) {
    free(node);
    node = NULL;
  }

  return node;
}

bool trie_delete(Trie *trie, char *signedtext) {
  unsigned char *text = (unsigned char *)signedtext;
  bool result = false;

  if (trie == NULL) {
    return false;
  }

  trie = trie_node_delete(trie, text, &result);
  return result;
}

bool trie_search(Trie *trie, char *signedtext) {
  unsigned char *text = (unsigned char *)signedtext;
  int length = strlen(signedtext);
  TrieNode *tmp = trie;

  for (int i = 0; i < length; i++) {
    unsigned char c = text[i];
    if (tmp->children[c] == NULL) {
      return false;
    }
    tmp = tmp->children[c];
  }

  if (tmp->terminal) {
    return true;
  }

  return false;
}

void trie_node_print(TrieNode *node, unsigned char *prefix, int length) {
  unsigned char newprefix[length + 2];
  memcpy(newprefix, prefix, length);
  newprefix[length + 1] = '\0';

  if (node->terminal) {
    printf("%s\n", prefix);
  }

  for (int i = 0; i < NUM_CHARS; i++) {
    if (node->children[i] != NULL) {
      newprefix[length] = i;
      trie_node_print(node->children[i], newprefix, length + 1);
    }
  }
}

void trie_print(Trie *trie) { trie_node_print(trie, NULL, 0); }
