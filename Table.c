#include <stdlib.h>

#include "Table.h"

struct TAB_entry {
  void *key;
  void *value;
};

struct TAB_table_ {
  int len;
  int capacity;
  struct TAB_entry *stack;
};

TAB_table TAB_empty(void) {
  TAB_table t = malloc(sizeof *t);
  t->len = 0;
  t->capacity = 0;
  t->stack = NULL;
  return t;
}

void TAB_enter(TAB_table t, void *key, void *value) {
  if (t->len == t->capacity) {
    if (t->capacity == 0)
      t->capacity = 1;
    else
      t->capacity <<= 1;
    t->stack = realloc(t->stack, sizeof(struct TAB_entry) * t->capacity);
  }
  t->stack[t->len].key = key;
  t->stack[t->len++].value = value;
}

void *TAB_look(TAB_table t, void *key) {
  int i;
  for (i = t->len-1; i >= 0; --i)
    if (key == t->stack[i].key)
      return t->stack[i].value;
  return NULL;
}

void *TAB_pop(TAB_table t) {
  void *k = t->stack[--t->len].key;
  return k;
}
