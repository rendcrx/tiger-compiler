typedef struct TAB_table_ *TAB_table;

TAB_table TAB_empty(void);
void TAB_enter(TAB_table t, void *key, void *value);
void *TAB_look(TAB_table t, void *key);
void *TAB_pop(TAB_table t);
