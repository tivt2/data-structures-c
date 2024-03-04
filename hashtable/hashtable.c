#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { HT_INTEGER, HT_STRING } HTValueType;

typedef struct {
  char *key;
  union {
    int integer;
    char *string;
  } value;
} HTEntry;

typedef struct HTNode {
  HTEntry entry;
  struct HTNode *next;
} HTNode;

typedef struct {
  size_t capacity;
  size_t len;
  HTNode **data;
} HashTable;

bool ht_insert(HashTable *ht, HTEntry entry);

HashTable *ht_create(size_t initial_capacity) {
  HashTable *ht = malloc(sizeof(HashTable));
  if (!ht) {
    return NULL;
  }

  ht->data = malloc(sizeof(HTNode *) * initial_capacity);
  if (!ht->data) {
    free(ht);
    return NULL;
  }
  ht->capacity = initial_capacity;
  ht->len = 0;

  for (size_t i = 0; i < (ht->capacity); i++) {
    ht->data[i] = NULL;
  }

  return ht;
}

void ht_node_destroy(HTNode *node) {
  if (!node) {
    return;
  }
  ht_node_destroy(node->next);
  free(node);
}

void ht_destroy(HashTable *ht) {
  for (size_t i = 0; i < (ht->capacity); i++) {
    if (ht->data[i]) {
      ht_node_destroy(ht->data[i]);
    }
  }
  free(ht->data);
  free(ht);
}

size_t ht_load_factor(HashTable *ht) { return (ht->len * 100) / ht->capacity; }

size_t ht_hash_djb2(HashTable *ht, char *key) {
  size_t hash = 5381;
  int c;
  while ((c = *key++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % ht->capacity;
}

bool ht_grow(HashTable *ht) {
  size_t old_capacity = ht->capacity;
  ht->capacity *= 2;

  HTNode **new_data = malloc(sizeof(HTNode *) * ht->capacity);
  if (!new_data) {
    return false;
  }

  for (size_t i = 0; i < (ht->capacity); i++) {
    new_data[i] = NULL;
  }

  for (size_t i = 0; i < old_capacity; i++) {
    HTNode *curr = ht->data[i];
    while (curr) {
      HTNode *next = curr->next;
      size_t hash = ht_hash_djb2(ht, curr->entry.key);

      HTNode *new_curr = new_data[hash];
      if (!new_curr) {
        new_data[hash] = curr;
      } else {
        while (new_curr->next) {
          new_curr = new_curr->next;
        }
        new_curr->next = curr;
      }

      curr = next;
    }
  }

  free(ht->data);
  ht->data = new_data;
  return true;
}

size_t ht_capacity(HashTable *ht) { return ht->capacity; }

// return true for new insertion, false for value override/malloc NULL
bool ht_insert(HashTable *ht, HTEntry entry) {
  if (ht_load_factor(ht) >= 80) {
    if (!ht_grow(ht)) {
      return false;
    }
  }

  HTNode *new_node = malloc(sizeof(HTNode));
  if (!new_node) {
    perror("ht_insert malloc");
    return false;
  }
  new_node->entry = entry;
  new_node->next = NULL;

  size_t hash = ht_hash_djb2(ht, entry.key);
  HTNode *curr = ht->data[hash];
  if (!curr) {
    ht->data[hash] = new_node;
  } else {
    HTNode *prev = NULL;

    while (curr) {
      if (strcmp(curr->entry.key, entry.key) == 0) {
        curr->entry = entry;
        free(new_node);
        return true;
      }

      prev = curr;
      curr = curr->next;
    }

    prev->next = new_node;
  }

  ht->len++;
  return true;
}

HTEntry ht_get(HashTable *ht, char *key) {
  size_t hash = ht_hash_djb2(ht, key);
  HTNode *curr = ht->data[hash];

  while (curr) {
    if (strcmp(curr->entry.key, key) == 0) {
      return curr->entry;
    }

    curr = curr->next;
  }

  return (HTEntry){.key = NULL, .value = 0};
}

bool ht_delete(HashTable *ht, char *key) {
  size_t hash = ht_hash_djb2(ht, key);
  HTNode *curr = ht->data[hash];
  HTNode *prev = NULL;

  while (curr) {
    if (strcmp(curr->entry.key, key) == 0) {
      if (prev) {
        prev->next = curr->next;
      } else {
        ht->data[hash] = curr->next;
      }
      free(curr);
      ht->len--;
      return true;
    }

    prev = curr;
    curr = curr->next;
  }

  return false;
}

void ht_node_print(HTNode *node) {
  printf("[key: %s, val: %d]\n", node->entry.key, node->entry.value.integer);

  if (node->next) {
    ht_node_print(node->next);
  }
}

void ht_print(HashTable *ht) {
  for (size_t i = 0; i < (ht->capacity); i++) {
    if (ht->data[i]) {
      ht_node_print(ht->data[i]);
    }
  }
  printf("capacity: %zu\n", ht->capacity);
}
