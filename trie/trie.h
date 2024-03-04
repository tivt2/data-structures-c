#ifndef TRIE
#define TRIE

#include <stdbool.h>
#include <stddef.h>

typedef struct Trie Trie;

Trie *trie_create();

void trie_destroy(Trie *trie);

bool trie_insert(Trie *trie, char *value);

bool trie_delete(Trie *trie, char *value);

bool trie_search(Trie *trie, char *value);

void trie_print(Trie *trie);

#endif // !TRIE
