/* program    → ε | exp */
struct A_Exp;
struct A_Condexp;
struct A_Letexp;
struct A_Lvalue;
struct A_Arglist;
struct A_Nonarglist;
struct A_Reclist;
struct A_Nonreclist;
struct A_Expseq;
struct A_Decs;
struct A_Dec;
struct A_Tydecs;
struct A_Tydec;
struct A_Ty;
struct A_Tyfields;
struct A_Tyfields1;
struct A_Tyfield;
struct A_Vardec;
struct A_Fundecs;
struct A_Fundec;

struct A_Program {
  enum {
    EPSILON,
    EXP
  } kind;

  struct A_Exp *exp;
};

/* exp         → INT */
/*             | STRING */
/*             | NIL */
/*             | id ( arglist ) */
/*             | lvalue */
/*             | ( expseq ) */
/*             | condexp */
/*             | letexp */
/*             | exp OR exp */
/*             | exp AND exp */
/*             | exp < exp */
/*             | exp > exp */
/*             | exp <= exp */
/*             | exp >= exp */
/*             | exp + exp */
/*             | exp - exp */
/*             | exp * exp */
/*             | exp / exp */
/*             | exp = exp */
/*             | exp <> exp */
/*             | id [ exp ] OF exp */
/*             | id { reclist } */
/*             | lvalue := exp */
/*             | - exp */
/*             | BREAK */
struct A_Exp {
  enum {
    A_EXP_INT,
    A_EXP_STRING,
    A_EXP_NIL,
    A_EXP_ID,
    A_EXP_LVALUE,
    A_EXP_EXPSEQ,
    A_EXP_CONDEXP,
    A_EXP_LETEXP,
    A_EXP_OR,
    A_EXP_AND,
    A_EXP_LT,
    A_EXP_GT,
    A_EXP_LE,
    A_EXP_GE,
    A_EXP_ADD,
    A_EXP_SUB,
    A_EXP_TIMES,
    A_EXP_DIV,
    A_EXP_EQ,
    A_EXP_LG,
    A_EXP_OF,
    A_EXP_RECLIST,
    A_EXP_ASSIGN,
    A_EXP_SINGLESUB,
    A_EXP_BREAK
  } kind;

  union {
    int num;                                                       // INT
    char str[20];                                                  // STRING
    struct {char id[20]; struct A_Arglist *arglist;} id;           // ID
    struct A_Lvalue *lval;                                         // LVALUE
    struct A_Expseq *expseq;                                       // EXPSEQ
    struct A_Condexp *condexp;                                     // CONDEXP
    struct A_Letexp *letexp;                                       // LETEXP
    struct {struct A_Exp *left, *right;} or;                       // OR
    struct {struct A_Exp *left, *right;} and;                      // AND
    struct {struct A_Exp *left, *right;} lt;                       // LT
    struct {struct A_Exp *left, *right;} gt;                       // GT
    struct {struct A_Exp *left, *right;} le;                       // LE
    struct {struct A_Exp *left, *right;} ge;                       // GE
    struct {struct A_Exp *left, *right;} add;                      // ADD
    struct {struct A_Exp *left, *right;} sub;                      // SUB
    struct {struct A_Exp *left, *right;} times;                    // TIMES
    struct {struct A_Exp *left, *right;} div;                      // DIV
    struct {struct A_Exp *left, *right;} eq;                       // EQ
    struct {struct A_Exp *left, *right;} lg;                       // LG
    struct {char id[20]; struct A_Exp *left, *right;} of;          // OF
    struct {char id[20]; struct A_Reclist *reclist;} reclist;      // RECLIST
    struct {struct A_Lvalue *lvalue; struct A_Exp *exp;} assign;   // ASSIGN
    struct A_Exp *exp;                                             // SINGLESUB
  } u;
};

/* condexp     → IF exp THEN exp ELSE exp */
/*             | IF exp THEN exp */
/*             | WHILE exp DO exp */
/*             | FOR id := exp TO exp DO exp */
struct A_Condexp {
  enum {
    A_CONDEXP_IFELSE,
    A_CONDEXP_IFTHEN,
    A_CONDEXP_WHILE,
    A_CONDEXP_FOR
  } kind;

  union {
    struct {struct A_Exp *e1, *e2, *e3;} ifelse;              // IFELSE
    struct {struct A_Exp *e1, *e2;} ifthen;                   // IFTHEN
    struct {struct A_Exp *e1, *e2;} whi;                      // WHILE
    struct {char id[20]; struct A_Exp *e1, *e2, *e3;} fo;     // FOR
  } u;
};

/* letexp      → LET decs IN expseq END */
struct A_Letexp {
  struct A_Decs *decs;
  struct A_Expseq *expseq;
};

/* lvalue      → id */
/*             | id [ exp ] */
/*             | lvalue [ exp ] */
/*             | lvalue . id */
struct A_Lvalue {
  enum {
    A_LVALUE_ID,
    A_LVALUE_IDEXP,
    A_LVALUE_LEXP,
    A_LVALUE_LID
  } kind;

  union {
    char id[20];
    struct {char id[20]; struct A_Exp *exp;} idexp; 
    struct {struct A_Lvalue *lvalue; struct A_Exp *exp;} lexp;
    struct {struct A_Lvalue *lvalue; char id[20];} lid;
  } u;
};

/* arglist     → ε | nonarglist */
/* nonarglist  → exp | exp , nonarglist */
struct A_Arglist {
  enum {
    A_ARGLIST_EPSILON,
    A_ARGLIST_NONARGLIST
  } kind;

  struct A_Nonarglist *nonarglist;
};

struct A_Nonarglist {
  enum {
    A_NONARGLIST_EXP,
    A_NONARGLIST_EXPNONARGLIST
  } kind;

  union {
    struct A_Exp *exp;
    struct {struct A_Exp *exp; struct A_Nonarglist *arglist;} expnonarglist;
  } u;
};

/* reclist     → ε | nonreclist */
/* nonreclist  → id = exp | id = exp , nonreclist */
struct A_Reclist {
  enum {
    A_RECLIST_EPSILON,
    A_RECLIST_NONRECLIST
  } kind;
  struct A_Nonreclist *nonreclist;
};

struct A_Nonreclist {
  enum {
    A_NONRECLIST_IDEXP,
    A_NONRECLIST_NONRECLIST,
  } kind;

  union {
    struct {char id[20]; struct A_Exp *exp;} idexp;
    struct {char id[20]; struct A_Exp *exp; struct A_Nonreclist *nonreclist;} nonreclist;
  } u;
};

/* expseq      → ε | exp | exp ; expseq */
struct A_Expseq {
  enum {
    A_EXPSEQ_EPSILON,
    A_EXPSEQ_EXP,
    A_EXPSEQ_EXPSEQ
  } kind;

  union {
    struct A_Exp *exp;
    struct {struct A_Exp *exp; struct A_Expseq *expseq;} expseq;
  } u;
};

/* decs        → ε | dec decs */
/* dec         → tydecs | vardec | fundecs */
struct A_Decs {
  enum {
    A_DECS_EPSILON,
    A_DECS_DECS
  } kind;
  struct A_Dec *dec;
  struct A_Decs *decs;
};

struct A_Dec {
  enum {
    A_DEC_TYDECS,
    A_DEC_VARDEC,
    A_DEC_FUNDECS,
  } kind;

  union {
    struct A_Tydecs *tydecs;
    struct A_Vardec *vardec;
    struct A_Fundecs *fundecs;
  } u;
};

/* tydecs      → tydec | tydec tydecs */
/* tydec       → TYPE id = ty */
struct A_Tydecs {
  enum {
    A_TYDECS_TYDEC,
    A_TYDECS_TYDECS,
  } kind;

  union {
    struct A_Tydec *tydec;
    struct {struct A_Tydec *tydec; struct A_Tydecs *tydecs;} tydecs;
  } u;
};

struct A_Tydec {
  char id[20];
  struct A_Ty *ty;
};

/* ty          → id */
/*             | { tyfields } */
/*             | ARRAY OF id */
struct A_Ty {
  enum {
    A_TY_ID,
    A_TY_TYFIELDS,
    A_TY_OF
  } kind;

  union {
    char id[20];
    struct A_Tyfields *tyfields;
    char ofid[20];
  } u;
};

/* tyfields    → ε | tyfields1 */
/* tyfields1   → tyfield | tyfield , tyfields1 */
/* tyfield     → id : id */
struct A_Tyfields {
  enum {
    A_TYFIELDS_EPSILON,
    A_TYFIELDS_TYFIELDS1
  } kind;

  struct A_Tyfields1 *tyfields1;
};

struct A_Tyfields1 {
  enum {
    A_TYFIELDS1_TYFIELD,
    A_TYFIELDS1_TYFIELD1,
  } kind;

  union {
    struct A_Tyfield *tyfield;
    struct {struct A_Tyfield *tyfield; struct A_Tyfields1 *tyfields1;} tyfield1;
  } u;
};

struct A_Tyfield {
  char id1[20];
  char id2[20];
};

/* vardec      → VAR id := exp */
/*             | VAR id : id := exp */
struct A_Vardec {
  enum {
    A_VARDEC_ID,
    A_VARDEC_IDID,
  } kind;

  union {
    struct {char id[20]; struct A_Exp *exp;} id;
    struct {char id1[20]; char id2[20]; struct A_Exp *exp;} idid;
  } u;
};

/* fundecs     → fundec | fundec fundecs */
/* fundec      → FUNCTION id ( tyfields ) = exp */
/*             | FUNCTION id ( tyfields ) : id = exp */
struct A_Fundecs {
  enum {
    A_FUNDECS_FUNDEC,
    A_FUNDECS_FUNDECS,
  } kind;

  union {
    struct A_Fundec *fundec;
    struct {struct A_Fundec *fundec; struct A_Fundecs *fundecs;} fundecs;
  } u;
};

struct A_Fundec {
  enum {
    A_FUNDEC_ID,
    A_FUNDEC_IDID,
  } kind;

  union {
    struct {char id[20]; struct A_Tyfields *tyfields; struct A_Exp *exp;} id;
    struct {char id1[20]; struct A_Tyfields *tyfields; char id2[20]; struct A_Exp *exp;} idid;
  } u;
};

struct A_Program *A_Program_Epsilon();
struct A_Program *A_Program_Exp(struct A_Exp *exp);

struct A_Exp *A_Exp_Int(int num);
struct A_Exp *A_Exp_String(char *str);
struct A_Exp *A_Exp_Nil();
struct A_Exp *A_Exp_Id(char *id, struct A_Arglist *arglist);
struct A_Exp *A_Exp_Lvalue(struct A_Lvalue *lval);
struct A_Exp *A_Exp_Expseq(struct A_Expseq *expseq);
struct A_Exp *A_Exp_Condexp(struct A_Condexp *condexp);
struct A_Exp *A_Exp_Letexp(struct A_Letexp *letexp);
struct A_Exp *A_Exp_Or(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_And(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Lt(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Gt(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Le(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Ge(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Add(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Sub(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Times(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Div(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Eq(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Lg(struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Of(char id[], struct A_Exp *left, struct A_Exp *right);
struct A_Exp *A_Exp_Reclist(char id[], struct A_Reclist *reclist);
struct A_Exp *A_Exp_Assign(struct A_Lvalue *lvalue, struct A_Exp *exp);
struct A_Exp *A_Exp_Singlesub(struct A_Exp *exp);
struct A_Exp *A_Exp_Break();

struct A_Condexp *A_Condexp_Ifelse(struct A_Exp *e1, struct A_Exp *e2, struct A_Exp *e3);
struct A_Condexp *A_Condexp_Ifthen(struct A_Exp *e1, struct A_Exp *e2);
struct A_Condexp *A_Condexp_While(struct A_Exp *e1, struct A_Exp *e2);
struct A_Condexp *A_Condexp_For(char id[], struct A_Exp *e1, struct A_Exp *e2, struct A_Exp *e3);

struct A_Letexp *A_Letexp(struct A_Decs *decs, struct A_Expseq *expseq);

struct A_Lvalue *A_Lvalue_Id(char id[]);
struct A_Lvalue *A_Lvalue_Idexp(char *id, struct A_Exp *exp);
struct A_Lvalue *A_Lvalue_Lexp(struct A_Lvalue *lvalue, struct A_Exp *exp);
struct A_Lvalue *A_Lvalue_Lid(struct A_Lvalue *lvalue, char *id);

struct A_Arglist *A_Arglist_Epsilon();
struct A_Arglist *A_Arglist_Nonarglist(struct A_Nonarglist *nonarglist);

struct A_Nonarglist *A_Nonarglist_Exp(struct A_Exp *exp);
struct A_Nonarglist *A_Nonarglist_Expnonarglist(struct A_Exp *exp, struct A_Nonarglist *arglist);

struct A_Reclist *A_Reclist_Epsilon();
struct A_Reclist *A_Reclist_Nonreclist(struct A_Nonreclist *nonreclist);

struct A_Nonreclist *A_Nonreclist_Idexp(char *id, struct A_Exp *exp);
struct A_Nonreclist *A_Nonreclist_Nonreclist(char *id, struct A_Exp *exp, struct A_Nonreclist *nonreclist);

struct A_Expseq *A_Expseq_Epsilon();
struct A_Expseq *A_Expseq_Exp(struct A_Exp *exp);
struct A_Expseq *A_Expseq_Expseq(struct A_Exp *exp, struct A_Expseq *expseq);

struct A_Decs *A_Decs_Epsilon();
struct A_Decs *A_Decs_Decs(struct A_Dec *dec, struct A_Decs *decs);

struct A_Dec *A_Dec_Tydecs(struct A_Tydecs *tydecs);
struct A_Dec *A_Dec_Vardec(struct A_Vardec *vardec);
struct A_Dec *A_Dec_Fundecs(struct A_Fundecs *fundecs);

struct A_Tydecs *A_Tydecs_Tydec(struct A_Tydec *tydec);
struct A_Tydecs *A_Tydecs_Tydecs(struct A_Tydec *tydec, struct A_Tydecs *tydecs);

struct A_Tydec *A_Tydec(char *id, struct A_Ty *ty);

struct A_Ty *A_Ty_Id(char *id);
struct A_Ty *A_Ty_Tyfields(struct A_Tyfields *tyfields);
struct A_Ty *A_Ty_Of(char *ofid);

struct A_Tyfields *A_Tyfields_Epsilon();
struct A_Tyfields *A_Tyfields_Tyfields(struct A_Tyfields1 *tyfields1);

struct A_Tyfields1 *A_Tyfields1_Tyfield(struct A_Tyfield *tyfield);
struct A_Tyfields1 *A_Tyfields1_Tyfield1(struct A_Tyfield *tyfield, struct A_Tyfields1 *tyfields1);

struct A_Tyfield *A_Tyfield(char *id1, char *id2);

struct A_Vardec *A_Vardec_Id(char *id, struct A_Exp *exp);
struct A_Vardec *A_Vardec_Idid(char *id1, char *id2, struct A_Exp *exp);

struct A_Fundecs *A_Fundecs_Fundec(struct A_Fundec *fundec);
struct A_Fundecs *A_Fundecs_Fundecs(struct A_Fundec *fundec, struct A_Fundecs *fundecs);

struct A_Fundec *A_Fundec_Id(char *id, struct A_Tyfields *tyfields, struct A_Exp *exp);
struct A_Fundec *A_Fundec_Idid(char *id1, struct A_Tyfields *tyfields, char *id2, struct A_Exp *exp);

struct A_Exp *ParseExp();
struct A_Condexp *ParseCondexp();
struct A_Letexp *ParseLetexp();
struct A_Lvalue *ParseLvalue();
struct A_Arglist *ParseArglist();
struct A_Nonarglist *ParseNonarglist();
struct A_Reclist *ParseReclist();
struct A_Nonreclist *ParseNonreclist();
struct A_Expseq *ParseExpseq();
struct A_Decs *ParseDecs();
struct A_Dec *ParseDec();
struct A_Tydecs *ParseTydecs();
struct A_Tydec *ParseTydec();
struct A_Ty *ParseTy();
struct A_Tyfields *ParseTyfields();
struct A_Tyfields1 *ParseTyfields1();
struct A_Tyfield *ParseTyfield();
struct A_Vardec *ParseVardec();
struct A_Fundecs *ParseFundecs();
struct A_Fundec *ParseFundec();
struct A_Program *ParseProgram();
