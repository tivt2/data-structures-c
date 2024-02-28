#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct ListNode {
  ListValueIn value;
  struct ListNode *next;
} ListNode;

typedef struct LinkedList {
  ListNode *head;
  ListNode *tail;
  size_t size;
} LinkedList;

LinkedList *ll_create(void) {
  LinkedList *ll = malloc(sizeof(LinkedList));
  if (!ll) {
    return NULL;
  }
  ll->head = NULL;
  ll->tail = NULL;
  ll->size = 0;

  return ll;
}

void ll_destroy(LinkedList *ll) {
  ListNode *curr = ll->head;
  while (curr) {
    ListNode *next = curr->next;
    free(curr);
    curr = next;
  }
  free(ll);
}

bool ll_valuecmp(ListValueIn v1, ListValueIn v2) {
  switch (v1.type) {
  case LL_INTEGER:
    return v1.int_val == v2.int_val;
  case LL_STRING:
    return strcmp(v1.string, v2.string);
  }
}

size_t ll_size(LinkedList *ll) { return ll->size; }

bool ll_add(LinkedList *ll, ListValueIn value) {
  ListNode *node = malloc(sizeof(ListNode));
  if (!node) {
    return 0;
  }
  node->value = value;
  node->next = NULL;

  if (!ll->head) {
    ll->head = ll->tail = node;
  } else {
    ll->tail->next = node;
    ll->tail = node;
  }

  ll->size++;
  return 1;
}

bool ll_add_at(LinkedList *ll, ListValueIn value, size_t idx) {
  if (idx >= ll->size) {
    return 0;
  }

  ListNode *node = malloc(sizeof(ListNode));
  if (!node) {
    return 0;
  }
  node->value = value;

  ListNode *curr = ll->head;
  while (idx > 0) {
    curr = curr->next;
    idx--;
  }

  node->next = curr->next;
  curr->next = node;

  ll->size++;
  return 1;
}

bool ll_remove(LinkedList *ll, ListValueIn value) {
  ListNode *curr = ll->head;
  ListNode *prev = NULL;

  while (curr && !ll_valuecmp(curr->value, value)) {
    prev = curr;
    curr = curr->next;
  }
  if (!curr) {
    return 0;
  }

  if (curr == ll->head) {
    ll->head = curr->next;
    curr->next = NULL;
  } else if (curr == ll->tail) {
    ll->tail = prev;
    prev->next = NULL;
  } else {
    prev->next = curr->next;
    curr->next = NULL;
  }

  free(curr);

  ll->size--;
  return 1;
}

bool ll_remove_at(LinkedList *ll, size_t idx) {
  if (idx >= ll->size) {
    return 0;
  }
  ListNode *curr = ll->head;
  ListNode *prev = NULL;

  while (idx > 0) {
    prev = curr;
    curr = curr->next;
    idx--;
  }

  if (curr == ll->head) {
    ll->head = curr->next;
    curr->next = NULL;
  } else if (curr == ll->tail) {
    ll->tail = prev;
    prev->next = NULL;
  } else {
    prev->next = curr->next;
    curr->next = NULL;
  }

  free(curr);

  ll->size--;
  return 1;
}

ListValueOut ll_get_at(LinkedList *ll, size_t idx) {
  ListValueOut out;
  if (idx >= ll->size) {
    out.isValid = false;
    return out;
  }

  ListNode *curr = ll->head;
  while (idx > 0) {
    curr = curr->next;
    idx--;
  }

  out.isValid = true;
  out.type = curr->value.type;
  switch (out.type) {
  case LL_INTEGER:
    out.int_val = curr->value.int_val;
    break;
  case LL_STRING:
    out.string = curr->value.string;
    break;
  }

  return out;
}

void ll_print(LinkedList *ll) {
  ListNode *curr = ll->head;
  printf("[");
  while (curr) {
    if (curr != ll->head) {
      printf(", ");
    }
    switch (curr->value.type) {
    case LL_INTEGER:
      printf("%d", curr->value.int_val);
      break;
    case LL_STRING:
      printf("%s", curr->value.string);
      break;
    }
    curr = curr->next;
  }
  printf("]\n");
}
