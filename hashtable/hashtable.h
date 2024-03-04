#ifndef HASHMAP
#define HASHMAP

#include <stdbool.h>
#include <stddef.h>

typedef enum { HT_INTEGER, HT_STRING } HTValueType;

typedef struct {
  char *key;
  union {
    int integer;
    char *string;
  } value;
} HTEntry;

typedef struct HashTable HashTable;

HashTable *ht_create(size_t initial_capacity);

void ht_destroy(HashTable *ht);

size_t ht_capacity(HashTable *ht);

// return true for new insertion, false for value override/malloc NULL
bool ht_insert(HashTable *ht, HTEntry entry);

HTEntry ht_get(HashTable *ht, char *key);

bool ht_delete(HashTable *ht, char *key);

void ht_print(HashTable *ht);

#endif // !HASHMAP
