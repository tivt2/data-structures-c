#ifndef LINKED_LIST
#define LINKED_LIST

#include <stddef.h>
#include <stdbool.h>

typedef enum { LL_INTEGER, LL_STRING } ListValueType;

typedef struct {
  ListValueType type;
  union {
    int int_val;
    char *string;
  };
} ListValueIn;

typedef struct {
  bool isValid;
  ListValueType type;
  union {
    int int_val;
    char *string;
  };
} ListValueOut;

typedef struct LinkedList LinkedList;

LinkedList *ll_create(void);

void ll_destroy(LinkedList *ll);

size_t ll_size(LinkedList *ll);

bool ll_add(LinkedList *ll, ListValueIn value);

bool ll_add_at(LinkedList *ll, ListValueIn value, size_t idx);

bool ll_remove(LinkedList *ll, ListValueIn value);

bool ll_remove_at(LinkedList *ll, size_t idx);

ListValueOut ll_get_at(LinkedList *ll, size_t idx);

void ll_print(LinkedList *ll);

#endif // !LINKED_LIST
