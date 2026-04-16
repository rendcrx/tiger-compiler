#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "TigerAST.h"
#include "Lex.h"

static struct Token *curtok;
static int i;

static void nexttok() {
  if (i >= tokens_size)
    curtok = NULL;
  else
    curtok = &tokens[i++];
}

static void expecttok(int class) {
  if (i >= tokens_size)
    curtok = NULL;
  else
    curtok = &tokens[i++];
  assert(curtok->tok == class);
}

static struct Token *peer() {
  if (i+1 >= tokens_size)
    return NULL;
  else
    return &tokens[i+1];
}

struct A_Program *A_Program_Epsilon() {
  struct A_Program *p = malloc(sizeof *p);
  p->kind = EPSILON;
  return p;
}

struct A_Program *A_Program_Exp(struct A_Exp *exp) {
  struct A_Program *p = malloc(sizeof *p);
  p->kind = EXP;
  p->exp = exp;
  return p;
}

struct A_Exp *A_Exp_Int(int num) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_INT;
  e->u.num = num;
  return e;
}

struct A_Exp *A_Exp_String(char *str) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_STRING;
  strcpy(e->u.str, str);
  return e;
}

struct A_Exp *A_Exp_Nil() {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_NIL;
  return e;
}

struct A_Exp *A_Exp_Id(char *id, struct A_Arglist *arglist) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_ID;
  strcpy(e->u.id.id, id);
  e->u.id.arglist = arglist;
  return e;
}

struct A_Exp *A_Exp_Lvalue(struct A_Lvalue *lval) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LVALUE;
  e->u.lval = lval;
  return e;
}

struct A_Exp *A_Exp_Expseq(struct A_Expseq *expseq) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_EXPSEQ;
  e->u.expseq = expseq;
  return e;
}

struct A_Exp *A_Exp_Condexp(struct A_Condexp *condexp) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_CONDEXP;
  e->u.condexp = condexp;
  return e;
}

struct A_Exp *A_Exp_Letexp(struct A_Letexp *letexp) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LETEXP;
  e->u.letexp = letexp;
  return e;
}

struct A_Exp *A_Exp_Or(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_OR;
  e->u.or.left = left;
  e->u.or.right = right;
  return e;
}

struct A_Exp *A_Exp_And(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_AND;
  e->u.and.left = left;
  e->u.and.right = right;
  return e;
}

struct A_Exp *A_Exp_Lt(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LT;
  e->u.lt.left = left;
  e->u.lt.right = right;
  return e;
}

struct A_Exp *A_Exp_Gt(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_GT;
  e->u.gt.left = left;
  e->u.gt.right = right;
  return e;
}

struct A_Exp *A_Exp_Le(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LE;
  e->u.le.left = left;
  e->u.le.right = right;
  return e;
}

struct A_Exp *A_Exp_Ge(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_GE;
  e->u.ge.left = left;
  e->u.ge.right = right;
  return e;
}

struct A_Exp *A_Exp_Add(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_ADD;
  e->u.add.left = left;
  e->u.add.right = right;
  return e;
}

struct A_Exp *A_Exp_Sub(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_SUB;
  e->u.sub.left = left;
  e->u.sub.right = right;
  return e;
}

struct A_Exp *A_Exp_Times(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_TIMES;
  e->u.times.left = left;
  e->u.times.right = right;
  return e;
}

struct A_Exp *A_Exp_Div(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_DIV;
  e->u.div.left = left;
  e->u.div.right = right;
  return e;
}

struct A_Exp *A_Exp_Eq(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_EQ;
  e->u.eq.left = left;
  e->u.eq.right = right;
  return e;
}

struct A_Exp *A_Exp_Lg(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LG;
  e->u.lg.left = left;
  e->u.lg.right = right;
  return e;
}

struct A_Exp *A_Exp_Of(char id[], struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_OF;
  strcpy(e->u.of.id, id);
  e->u.of.left = left;
  e->u.of.right = right;
  return e;
}

struct A_Exp *A_Exp_Reclist(char id[], struct A_Reclist *reclist) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_RECLIST;
  strcpy(e->u.reclist.id, id);
  e->u.reclist.reclist = reclist;
  return e;
}

struct A_Exp *A_Exp_Assign(struct A_Lvalue *lvalue, struct A_Exp *exp) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_ASSIGN;
  e->u.assign.lvalue = lvalue;
  e->u.assign.exp = exp;
  return e;
}

struct A_Exp *A_Exp_Singlesub(struct A_Exp *exp) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_SINGLESUB;
  e->u.exp = exp;
  return e;
}

struct A_Exp *A_Exp_Break() {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_BREAK;
  return e;
}

struct A_Condexp *A_Condexp_Ifelse(struct A_Exp *e1, struct A_Exp *e2, struct A_Exp *e3) {
  struct A_Condexp *c = malloc(sizeof *c);
  c->kind = A_CONDEXP_IFELSE;
  c->u.ifelse.e1 = e1;
  c->u.ifelse.e2 = e2;
  c->u.ifelse.e3 = e3;
  return c;
}

struct A_Condexp *A_Condexp_Ifthen(struct A_Exp *e1, struct A_Exp *e2) {
  struct A_Condexp *c = malloc(sizeof *c);
  c->kind = A_CONDEXP_IFTHEN;
  c->u.ifthen.e1 = e1;
  c->u.ifthen.e2 = e2;
  return c;
}

struct A_Condexp *A_Condexp_While(struct A_Exp *e1, struct A_Exp *e2) {
  struct A_Condexp *c = malloc(sizeof *c);
  c->kind = A_CONDEXP_WHILE;
  c->u.whi.e1 = e1;
  c->u.whi.e2 = e2;
  return c;
}

struct A_Condexp *A_Condexp_For(char id[], struct A_Exp *e1, struct A_Exp *e2, struct A_Exp *e3) {
  struct A_Condexp *c = malloc(sizeof *c);
  c->kind = A_CONDEXP_FOR;
  strcpy(c->u.fo.id, id);
  c->u.fo.e1 = e1;
  c->u.fo.e2 = e2;
  c->u.fo.e3 = e3;
  return c;
}


struct A_Letexp *A_Letexp(struct A_Decs *decs, struct A_Expseq *expseq) {
  struct A_Letexp *l = malloc(sizeof *l);
  l->decs = decs;
  l->expseq = expseq;
  return l;
}

struct A_Lvalue *A_Lvalue_Id(char id[]) {
  struct A_Lvalue *l = malloc(sizeof *l);
  l->kind = A_LVALUE_ID;
  strcpy(l->u.id, id);
  return l;
}

struct A_Lvalue *A_Lvalue_Idexp(char *id, struct A_Exp *exp, struct A_Lvalue_Tail *tail) {
  struct A_Lvalue *l = malloc(sizeof *l);
  l->kind = A_LVALUE_IDEXP;
  strcpy(l->u.idexp.id, id);
  l->u.idexp.exp = exp;
  l->u.idexp.tail = tail;
  return l;
}

struct A_Lvalue *A_Lvalue_Idid(char *id1, char *id2, struct A_Lvalue_Tail *tail) {
  struct A_Lvalue *l = malloc(sizeof *l);
  l->kind = A_LVALUE_IDID;
  strcpy(l->u.idid.id1, id1);
  strcpy(l->u.idid.id2, id2);
  l->u.idid.tail = tail;
  return l;
}

struct A_Lvalue_Tail *A_Lvalue_Tail_Exp(struct A_Exp *exp, struct A_Lvalue_Tail *tail) {
  struct A_Lvalue_Tail *l = malloc(sizeof *l);
  l->kind = A_LVALUE_TAIL_EXP;
  l->u.exp.exp = exp;
  l->u.exp.tail = tail;
  return l;
}

struct A_Lvalue_Tail *A_Lvalue_Tail_Id(char *id, struct A_Lvalue_Tail *tail) {
  struct A_Lvalue_Tail *l = malloc(sizeof *l);
  l->kind = A_LVALUE_TAIL_ID;
  strcpy(l->u.id.id, id);
  l->u.id.tail = tail;
  return l;
}

struct A_Lvalue_Tail *A_Lvalue_Tail_Epsilon() {
  struct A_Lvalue_Tail *l = malloc(sizeof *l);
  l->kind = A_LVALUE_TAIL_EPSILON;
  return l;
}

struct A_Arglist *A_Arglist_Epsilon() {
  struct A_Arglist *a = malloc(sizeof *a);
  a->kind = A_ARGLIST_EPSILON;
  return a;
}

struct A_Arglist *A_Arglist_Nonarglist(struct A_Nonarglist *nonarglist) {
  struct A_Arglist *a = malloc(sizeof *a);
  a->kind = A_ARGLIST_NONARGLIST;
  a->nonarglist = nonarglist;
  return a;
}

struct A_Nonarglist *A_Nonarglist_Exp(struct A_Exp *exp) {
  struct A_Nonarglist *n = malloc(sizeof *n);
  n->kind = A_NONARGLIST_EXP;
  n->u.exp = exp;
  return n;
}

struct A_Nonarglist *A_Nonarglist_Expnonarglist(struct A_Exp *exp, struct A_Nonarglist *arglist) {
  struct A_Nonarglist *n = malloc(sizeof *n);
  n->kind = A_NONARGLIST_EXPNONARGLIST;
  n->u.expnonarglist.exp = exp;
  n->u.expnonarglist.arglist = arglist;
  return n;
}

struct A_Reclist *A_Reclist_Epsilon() {
  struct A_Reclist *r = malloc(sizeof *r);
  r->kind = A_RECLIST_EPSILON;
  return r;
}

struct A_Reclist *A_Reclist_Nonreclist(struct A_Nonreclist *nonreclist) {
  struct A_Reclist *r = malloc(sizeof *r);
  r->kind = A_RECLIST_NONRECLIST;
  r->nonreclist = nonreclist;
  return r;
}

struct A_Nonreclist *A_Nonreclist_Idexp(char *id, struct A_Exp *exp) {
  struct A_Nonreclist *n = malloc(sizeof *n);
  n->kind = A_NONRECLIST_IDEXP;
  strcpy(n->u.idexp.id, id);
  n->u.idexp.exp = exp;
  return n;
}

struct A_Nonreclist *A_Nonreclist_Nonreclist(char *id, struct A_Exp *exp, struct A_Nonreclist *nonreclist) {
  struct A_Nonreclist *n = malloc(sizeof *n);
  n->kind = A_NONRECLIST_NONRECLIST;
  strcpy(n->u.nonreclist.id, id);
  n->u.nonreclist.exp = exp;
  n->u.nonreclist.nonreclist = nonreclist;
  return n;
}

struct A_Expseq *A_Expseq_Epsilon() {
  struct A_Expseq *e = malloc(sizeof *e);
  e->kind = A_EXPSEQ_EPSILON;
  return e;
}

struct A_Expseq *A_Expseq_Exp(struct A_Exp *exp) {
  struct A_Expseq *e = malloc(sizeof *e);
  e->kind = A_EXPSEQ_EXP;
  e->u.exp = exp;
  return e;
}

struct A_Expseq *A_Expseq_Expseq(struct A_Exp *exp, struct A_Expseq *expseq) {
  struct A_Expseq *e = malloc(sizeof *e);
  e->kind = A_EXPSEQ_EXPSEQ;
  e->u.expseq.exp = exp;
  e->u.expseq.expseq = expseq;
  return e;
}

struct A_Decs *A_Decs_Epsilon() {
  struct A_Decs *d = malloc(sizeof *d);
  d->kind = A_DECS_EPSILON;
  return d;
}

struct A_Decs *A_Decs_Decs(struct A_Dec *dec, struct A_Decs *decs) {
  struct A_Decs *d = malloc(sizeof *d);
  d->kind = A_DECS_DECS;
  d->dec = dec;
  d->decs = decs;
  return d;
}

struct A_Dec *A_Dec_Tydecs(struct A_Tydecs *tydecs) {
  struct A_Dec *d = malloc(sizeof *d);
  d->kind = A_DEC_TYDECS;
  d->u.tydecs = tydecs;
  return d;
}

struct A_Dec *A_Dec_Vardec(struct A_Vardec *vardec) {
  struct A_Dec *d = malloc(sizeof *d);
  d->kind = A_DEC_VARDEC;
  d->u.vardec = vardec;
  return d;
}

struct A_Dec *A_Dec_Fundecs(struct A_Fundecs *fundecs) {
  struct A_Dec *d = malloc(sizeof *d);
  d->kind = A_DEC_FUNDECS;
  d->u.fundecs = fundecs;
  return d;
}

struct A_Tydecs *A_Tydecs_Tydec(struct A_Tydec *tydec) {
  struct A_Tydecs *t = malloc(sizeof *t);
  t->kind = A_TYDECS_TYDEC;
  t->u.tydec = tydec;
  return t;
}

struct A_Tydecs *A_Tydecs_Tydecs(struct A_Tydec *tydec, struct A_Tydecs *tydecs) {
  struct A_Tydecs *t = malloc(sizeof *t);
  t->kind = A_TYDECS_TYDECS;
  t->u.tydecs.tydec = tydec;
  t->u.tydecs.tydecs = tydecs;
  return t;
}

struct A_Tydec *A_Tydec(char *id, struct A_Ty *ty) {
  struct A_Tydec *t = malloc(sizeof *t);
  strcpy(t->id, id);
  t->ty = ty;
  return t;
}

struct A_Ty *A_Ty_Id(char *id) {
  struct A_Ty *t = malloc(sizeof *t);
  t->kind = A_TY_ID;
  strcpy(t->u.id, id);
  return t;
}

struct A_Ty *A_Ty_Tyfields(struct A_Tyfields *tyfields) {
  struct A_Ty *t = malloc(sizeof *t);
  t->kind = A_TY_TYFIELDS;
  t->u.tyfields = tyfields;
  return t;
}

struct A_Ty *A_Ty_Of(char *ofid) {
  struct A_Ty *t = malloc(sizeof *t);
  t->kind = A_TY_OF;
  strcpy(t->u.ofid, ofid);
  return t;
}

struct A_Tyfields *A_Tyfields_Epsilon() {
  struct A_Tyfields *t = malloc(sizeof *t);
  t->kind = A_TYFIELDS_EPSILON;
  return t;
}

struct A_Tyfields *A_Tyfields_Tyfields(struct A_Tyfields1 *tyfields1) {
  struct A_Tyfields *t = malloc(sizeof *t);
  t->kind = A_TYFIELDS_TYFIELDS1;
  t->tyfields1 = tyfields1;
  return t;
}

struct A_Tyfields1 *A_Tyfields1_Tyfield(struct A_Tyfield *tyfield) {
  struct A_Tyfields1 *t = malloc(sizeof *t);
  t->kind = A_TYFIELDS1_TYFIELD;
  t->u.tyfield = tyfield;
  return t;
}

struct A_Tyfields1 *A_Tyfields1_Tyfield1(struct A_Tyfield *tyfield, struct A_Tyfields1 *tyfields1) {
  struct A_Tyfields1 *t = malloc(sizeof *t);
  t->kind = A_TYFIELDS1_TYFIELD1;
  t->u.tyfield1.tyfield = tyfield;
  t->u.tyfield1.tyfields1 = tyfields1;
  return t;
}

struct A_Tyfield *A_Tyfield(char *id1, char *id2) {
  struct A_Tyfield *t = malloc(sizeof *t);
  strcpy(t->id1, id1);
  strcpy(t->id2, id2);
  return t;
}

struct A_Vardec *A_Vardec_Id(char *id, struct A_Exp *exp) {
  struct A_Vardec *v = malloc(sizeof *v);
  v->kind = A_VARDEC_ID;
  strcpy(v->u.id.id, id);
  v->u.id.exp = exp;
  return v;
}

struct A_Vardec *A_Vardec_Idid(char *id1, char *id2, struct A_Exp *exp) {
  struct A_Vardec *v = malloc(sizeof *v);
  v->kind = A_VARDEC_IDID;
  strcpy(v->u.idid.id1, id1);
  strcpy(v->u.idid.id2, id2);
  v->u.idid.exp = exp;
  return v;
}

struct A_Fundecs *A_Fundecs_Fundec(struct A_Fundec *fundec) {
  struct A_Fundecs *f = malloc(sizeof *f);
  f->kind = A_FUNDECS_FUNDEC;
  f->u.fundec = fundec;
  return f;
}

struct A_Fundecs *A_Fundecs_Fundecs(struct A_Fundec *fundec, struct A_Fundecs *fundecs) {
  struct A_Fundecs *f = malloc(sizeof *f);
  f->kind = A_FUNDECS_FUNDECS;
  f->u.fundecs.fundec = fundec;
  f->u.fundecs.fundecs = fundecs;
  return f;
}

struct A_Fundec *A_Fundec_Id(char *id, struct A_Tyfields *tyfields, struct A_Exp *exp) {
  struct A_Fundec *f = malloc(sizeof *f);
  f->kind = A_FUNDEC_ID;
  strcpy(f->u.id.id, id);
  f->u.id.tyfields = tyfields;
  f->u.id.exp = exp;
  return f;
}

struct A_Fundec *A_Fundec_Idid(char *id1, struct A_Tyfields *tyfields, char *id2, struct A_Exp *exp) {
  struct A_Fundec *f = malloc(sizeof *f);
  f->kind = A_FUNDEC_IDID;
  strcpy(f->u.idid.id1, id1);
  f->u.idid.tyfields = tyfields;
  f->u.idid.exp = exp;
  strcpy(f->u.idid.id2, id2);
  return f;
}

static struct A_Exp *Parse_Primary_Exp() {
  int num;
  struct A_Exp *e, *p, *q;
  char *id, *id2;
  struct A_Reclist *r;
  struct A_Expseq *x;
  struct A_Lvalue *l;
  struct A_Lvalue_Tail *lt;
  struct A_Condexp *c;
  struct A_Decs *d;
  struct A_Expseq *s;
  switch (curtok->tok) {
    case TOK_INT:
      num = curtok->num;
      nexttok();
      return A_Exp_Int(num);
    case TOK_STR:
      id = curtok->id;
      nexttok();
      return A_Exp_String(id);
    case TOK_NIL:
      nexttok();
      return A_Exp_Nil();
    case TOK_ID:
      id = curtok->id;
      nexttok();
      switch (curtok->tok) {
        case '(':
          nexttok();
          e = A_Exp_Id(id, ParseArglist());
          assert(curtok->tok == ')');
          nexttok();
          return e;
        case '[':
          nexttok();
          e = ParseExp();
          assert(curtok->tok == ']');
          nexttok();
          switch (curtok->tok) {
            case TOK_OF:
              p = ParseExp();
              return A_Exp_Of(id, e, p);
            default:
              lt = ParseLvalueTail();
              l = A_Lvalue_Idexp(id, e, lt);
              goto assign;
          }
        case '{':
          nexttok();
          r = ParseReclist();
          assert(curtok->tok == '}');
          nexttok();
          return A_Exp_Reclist(id, r);
        case '.':
          expecttok(TOK_ID);
          id2 = curtok->id;
          lt = ParseLvalueTail();
          l = A_Lvalue_Idid(id, id2, lt);
          goto assign;
        default:
          l = A_Lvalue_Id(id);
          goto assign;
      }
    case '(':
      nexttok();
      x = ParseExpseq();
      assert(curtok->tok == ')');
      nexttok();
      return A_Exp_Expseq(x);
    case TOK_IF:
      nexttok();
      e = ParseExp();
      assert(curtok->tok == TOK_THEN);
      nexttok();
      p = ParseExp();
      if (curtok->tok == TOK_ELSE) {
        nexttok();
        q = ParseExp();
        c = A_Condexp_Ifelse(e, p, q);
        return A_Exp_Condexp(c);
      }
      c = A_Condexp_Ifthen(e, p);
      return A_Exp_Condexp(c);
    case TOK_WHILE:
      nexttok();
      e = ParseExp();
      assert(curtok->tok == TOK_DO);
      p = ParseExp();
      c = A_Condexp_While(e, p);
      return A_Exp_Condexp(c);
    case TOK_FOR:
      nexttok();
      assert(curtok->tok == TOK_ID);
      id = curtok->id;
      expecttok(TOK_ASSIGN);
      nexttok();
      e = ParseExp();
      assert(curtok->tok == TOK_TO);
      nexttok();
      p = ParseExp();
      assert(curtok->tok == TOK_DO);
      q = ParseExp();
      c = A_Condexp_For(id, e, p, q);
      return A_Exp_Condexp(c);
    case TOK_LET:
      nexttok();
      d = ParseDecs();
      assert(curtok->tok == TOK_IN);
      nexttok();
      s = ParseExpseq();
      assert(curtok->tok == TOK_END);
      nexttok();
      return A_Exp_Letexp(A_Letexp(d, s));
    case TOK_BREAK:
      nexttok();
      return A_Exp_Break();
    default: assert(0);
  }
assign:
  assert(curtok->tok == TOK_ASSIGN);
  p = ParseExp();
  return A_Exp_Assign(l, p);
}

static struct A_Exp *Parse_Unary_Exp() {
  if (curtok->tok == '-') {
    nexttok();
    return A_Exp_Singlesub(Parse_Primary_Exp());
  }
  return Parse_Primary_Exp();
}

static struct A_Exp *Parse_Mul_Exp() {
  struct A_Exp *left, *right;
  left = Parse_Unary_Exp();
  switch (curtok->tok) {
    case '*':
      nexttok();
      right = Parse_Mul_Exp();
      return A_Exp_Times(left, right);
    case '/':
      nexttok();
      right = Parse_Mul_Exp();
      return A_Exp_Div(left, right);
    default:
      return left;
  }
}

static struct A_Exp *Parse_Add_Exp() {
  struct A_Exp *left, *right;
  left = Parse_Mul_Exp();
  switch (curtok->tok) {
    case '+':
      nexttok();
      right = Parse_Add_Exp();
      return A_Exp_Add(left, right);
    case '-':
      nexttok();
      right = Parse_Add_Exp();
      return A_Exp_Sub(left, right);
    default: return left;
  }
}

static struct A_Exp *Parse_Cmp_Exp() {
  struct A_Exp *left, *right;
  left = Parse_Add_Exp();
  switch (curtok->tok) {
    case '<':
      nexttok();
      right = Parse_Add_Exp();
      return A_Exp_Lt(left, right);
    case '>':
      nexttok();
      right = Parse_Add_Exp();
      return A_Exp_Gt(left, right);
    case TOK_LE:
      nexttok();
      right = Parse_Add_Exp();
      return A_Exp_Le(left, right);
    case TOK_GE:
      nexttok();
      right = Parse_Add_Exp();
      return A_Exp_Ge(left, right);
    case '=':
      nexttok();
      right = Parse_Add_Exp();
      return A_Exp_Eq(left, right);
    case TOK_LG:
      nexttok();
      right = Parse_Add_Exp();
      return A_Exp_Lg(left, right);
    default: return left;
  }
}

static struct A_Exp *Parse_And_Exp() {
  struct A_Exp *left, *right;
  left = Parse_Cmp_Exp();
  if (curtok->tok == '&') {
    nexttok();
    right = Parse_And_Exp();
    return A_Exp_And(left, right);
  }
  return left;
}

static struct A_Exp *Parse_Or_Exp() {
  struct A_Exp *left, *right;
  left = Parse_And_Exp();
  if (curtok->tok == '|') {
    nexttok();
    right = Parse_Or_Exp();
    return A_Exp_Or(left, right);
  }
  return left;
}

struct A_Exp *ParseExp() {
  return Parse_Or_Exp();
}

struct A_Lvalue_Tail *ParseLvalueTail() {
  struct A_Exp *e;
  struct A_Lvalue_Tail *lt;
  char *id;
  if (curtok == NULL)
    return A_Lvalue_Tail_Epsilon();
  switch (curtok->tok) {
    case '[':
      nexttok();
      e = ParseExp();
      assert(curtok->tok == ']');
      nexttok();
      lt = ParseLvalueTail();
      return A_Lvalue_Tail_Exp(e, lt);
    case '.':
      nexttok();
      assert(curtok->tok == TOK_ID);
      id = curtok->id;
      nexttok();
      lt = ParseLvalueTail();
      return A_Lvalue_Tail_Id(id, lt);
    default: return A_Lvalue_Tail_Epsilon();
  }
}

struct A_Arglist *ParseArglist() {
  if (curtok == NULL)
    return A_Arglist_Epsilon();
  return A_Arglist_Nonarglist(ParseNonarglist());
}

struct A_Nonarglist *ParseNonarglist() {
  struct A_Exp *e;
  struct A_Nonarglist *n;
  e = ParseExp();
  if (curtok->tok == ',') {
    n = ParseNonarglist();
    return A_Nonarglist_Expnonarglist(e, n);
  }
  return A_Nonarglist_Exp(e);
}

struct A_Reclist *ParseReclist() {
  if (curtok == NULL)
    return A_Reclist_Epsilon();
  return A_Reclist_Nonreclist(ParseNonreclist());
}

struct A_Nonreclist *ParseNonreclist() {
  char *id;
  struct A_Exp *e;
  struct A_Nonreclist *n;
  assert(curtok->tok == TOK_ID);
  id = curtok->id;
  expecttok('=');
  nexttok();
  e = ParseExp();
  if (curtok && curtok->tok == ',') {
    nexttok();
    n = ParseNonreclist();
    return A_Nonreclist_Nonreclist(id, e, n);
  }
  return A_Nonreclist_Idexp(id, e);
}

struct A_Expseq *ParseExpseq() {
  struct A_Exp *e;
  struct A_Expseq *es;
  if (curtok == NULL)
    return A_Expseq_Epsilon();
  e = ParseExp();
  if (curtok->tok == ';') {
    nexttok();
    es = ParseExpseq();
    return A_Expseq_Expseq(e, es);
  }
  return A_Expseq_Exp(e);
}

struct A_Decs *ParseDecs() {
  struct A_Dec *d;
  struct A_Decs *ds;
  if (curtok == NULL)
    return A_Decs_Epsilon();
  d = ParseDec();
  ds = ParseDecs();
  return A_Decs_Decs(d, ds);
}

struct A_Dec *ParseDec() {
  switch (curtok->tok) {
    // tydecs
    case TOK_TYPE:
      return A_Dec_Tydecs(ParseTydecs());
    // vardec
    case TOK_VAR:
      return A_Dec_Vardec(ParseVardec());
    // fundecs:
    case TOK_FUNCTION:
      return A_Dec_Fundecs(ParseFundecs());
    default: assert(0); 
  }
}

struct A_Tydecs *ParseTydecs() {
  struct A_Tydec *d;
  struct A_Tydecs *ds;
  d = ParseTydec();
  if (curtok->tok == TOK_TYPE) {
    ds = ParseTydecs();
    return A_Tydecs_Tydecs(d, ds);
  }
  return A_Tydecs_Tydec(d);
}

struct A_Tydec *ParseTydec() {
  char *id;
  struct A_Ty *t;
  assert(curtok->tok == TOK_TYPE);
  nexttok();
  id = curtok->id;
  expecttok('=');
  nexttok();
  t = ParseTy();
  return A_Tydec(id, t);
}

struct A_Ty *ParseTy() {
  char *id;
  struct A_Tyfields *ty;
  switch (curtok->tok) {
    case TOK_ID:
      id = curtok->id;
      nexttok();
      return A_Ty_Id(id);
    case '{':
      nexttok();
      ty = ParseTyfields();
      expecttok('}');
      nexttok();
      return A_Ty_Tyfields(ty);
    case TOK_ARRAY:
      expecttok(TOK_OF);
      nexttok();
      assert(curtok->tok == TOK_ID);
      id = curtok->id;
      nexttok();
      return A_Ty_Of(id);
    default: assert(0);
  }
}

struct A_Tyfields *ParseTyfields() {
  if (curtok == NULL)
    return A_Tyfields_Epsilon();
  if (curtok->tok != TOK_ID)
    return A_Tyfields_Epsilon();
  return A_Tyfields_Tyfields(ParseTyfields1());
}

struct A_Tyfields1 *ParseTyfields1() {
  struct A_Tyfield *t;
  struct A_Tyfields1 *t1;
  t = ParseTyfield();
  if (curtok->tok == ',') {
    t1 = ParseTyfields1();
    return A_Tyfields1_Tyfield1(t, t1);
  }
  return A_Tyfields1_Tyfield(t);
}

struct A_Tyfield *ParseTyfield() {
  char *id, *id2;
  assert(curtok->tok == TOK_ID);
  id = curtok->id;
  expecttok(':');
  expecttok(TOK_ID);
  id2 = curtok->id;
  return A_Tyfield(id, id2);
}

struct A_Vardec *ParseVardec() {
  char *id, *id2;
  struct A_Exp *e;
  assert(curtok->tok == TOK_VAR);
  expecttok(TOK_ID);
  id = curtok->id;
  nexttok();
  if (curtok->tok == ':') {
    expecttok(TOK_ID);
    id2 = curtok->id;
    expecttok(TOK_ASSIGN);
    nexttok();
    e = ParseExp();
    return A_Vardec_Idid(id, id2, e);
  } else if (curtok->tok == TOK_ASSIGN) {
    nexttok();
    e = ParseExp();
    return A_Vardec_Id(id, e);
  }
  assert(0);
}

struct A_Fundecs *ParseFundecs() {
  struct A_Fundec *f;
  struct A_Fundecs *fs;
  f = ParseFundec();
  if (curtok->tok == TOK_FUNCTION) {
    fs = ParseFundecs();
    return A_Fundecs_Fundecs(f, fs);
  }
  return A_Fundecs_Fundec(f);
}

struct A_Fundec *ParseFundec() {
  char *id, *id2;
  struct A_Tyfields *ts;
  struct A_Exp *e;
  assert(curtok->tok == TOK_FUNCTION);
  expecttok(TOK_ID);
  id = curtok->id;
  expecttok('(');
  ts = ParseTyfields();
  assert(curtok->tok == ')');
  nexttok();
  if (curtok->tok == '=') {
    nexttok();
    e = ParseExp();
    return A_Fundec_Id(id, ts, e);
  } else if (curtok->tok == ':') {
    expecttok(TOK_ID);
    id2 = curtok->id;
    expecttok('=');
    nexttok();
    e = ParseExp();
    return A_Fundec_Idid(id, ts, id2, e);
  }
  assert(0);
}

struct A_Program *ParseProgram() {
  nexttok();
  switch (curtok->tok) {
    case 0: return A_Program_Epsilon();
    default: return A_Program_Exp(ParseExp());
  }
}
