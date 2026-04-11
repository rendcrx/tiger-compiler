#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"

typedef struct table *Table_;
struct table {
  string id;
  int value;
  Table_ tail;
};

Table_ Table(string id, int value, struct table *tail) {
  Table_ t = malloc(sizeof *t);
  t->id = id;
  t->value = value;
  t->tail = tail;
  return t;
}

int lookup(Table_ t, string key) {
  for (; t; t = t->tail) {
    if (strcmp(t->id, key) == 0)
      return t->value;
  }
  assert(0);
}

struct IntAndTable {
  int i;
  Table_ t;
};

typedef struct Ints *Ints_;

struct Ints {
  int i;
  Ints_ next;
};

struct IntsAndTable {
  Ints_ i;
  Table_ t;
};

struct IntAndTable interpExp(A_exp e, Table_ t);
struct IntsAndTable interpExpList(A_expList expList, Table_ t);

Table_ interpStm(A_stm s, Table_ t) {
  switch (s->kind) {
    case A_compoundStm:
      t = interpStm(s->u.compound.stm1, t);
      t = interpStm(s->u.compound.stm2, t);
      return t;
    case A_assignStm:
      struct IntAndTable i = interpExp(s->u.assign.exp, t);
      t = Table(s->u.assign.id, i.i, i.t);
      return t;
    case A_printStm:
      struct IntsAndTable j = interpExpList(s->u.print.exps, t);
      int first = 1;
      Ints_ is = j.i;
      while (is) {
        if (first) {
          printf("%d", is->i);
          first = 0;
        } else
          printf(" %d", is->i);
        is = is->next;
      }
      printf("\n");
      return j.t;
    default: assert(0);
  };
}

struct IntAndTable interpExp(A_exp e, Table_ t) {
  struct IntAndTable ret;
  switch (e->kind) {
    case A_idExp:
      ret.i = lookup(t, e->u.id);
      ret.t = t;
      return ret;
    case A_numExp:
      ret.i = e->u.num;
      ret.t = t;
      return ret;
    case A_opExp:
      ret = interpExp(e->u.op.left, t);
      int left = ret.i;
      ret = interpExp(e->u.op.right, ret.t);
      int right = ret.i;
      switch (e->u.op.oper) {
        case A_plus:
          ret.i = left + right;
          return ret;
        case A_minus:
          ret.i = left - right;
          return ret;
        case A_times:
          ret.i = left * right;
          return ret;
        case A_div:
          ret.i = left / right;
          return ret;
        default: assert(0);
      }
    case A_eseqExp:
      t = interpStm(e->u.eseq.stm, t);
      return interpExp(e->u.eseq.exp, t);
    default: assert(0);
  }
}

struct IntsAndTable interpExpList(A_expList expList, Table_ t) {
  struct IntsAndTable ret;
  switch (expList->kind) {
    case A_pairExpList:
      struct IntAndTable i = interpExp(expList->u.last, t);
      struct IntsAndTable j = interpExpList(expList->u.pair.expList, i.t);
      Ints_ v = malloc(sizeof *v);
      v->i = i.i;
      v->next = j.i;
      ret.i = v;
      ret.t = j.t;
      return ret;
    case A_lastExpList:
      struct IntAndTable k = interpExp(expList->u.last, t);
      Ints_ o = malloc(sizeof *v);
      o->i = k.i;
      o->next = NULL;
      ret.i = o;
      ret.t = k.t;
      return ret;
    default: assert(0);
  }
}

void interp(A_stm s) {
  interpStm(s, NULL);
}

int main() {
  A_stm prog = A_CompoundStm(A_AssignStm("a",
                                         A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
                             A_CompoundStm(A_AssignStm("b",
                                                       A_EseqExp(A_PrintStm(A_PairExpList(A_IdExp("a"),
                                                                                          A_LastExpList(A_OpExp(A_IdExp("a"),
                                                                                                                A_minus,
                                                                                                                A_NumExp(1))))),
                                                                 A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
                                           A_PrintStm(A_LastExpList(A_IdExp("b")))));

  interp(prog);
}
