enum {
  TOK_WHILE = 256,
  TOK_FOR,
  TOK_TO,
  TOK_BREAK,
  TOK_LET,
  TOK_IN,
  TOK_END,
  TOK_FUNCTION,
  TOK_VAR,
  TOK_TYPE,
  TOK_ARRAY,
  TOK_IF,
  TOK_THEN,
  TOK_ELSE,
  TOK_DO,
  TOK_OF,
  TOK_NIL,

  TOK_ID,

  TOK_ASSIGN,
  TOK_GE,
  TOK_LE,
  TOK_LG,

  TOK_INT,

  TOK_STR
};

struct Token {
  int tok;
  union {
    char id[20];
    int num;
  };
};

extern struct Token tokens[];
extern int tokens_size;

void GenTokens(char *str);
