//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#ifndef _AST_H_
#define _AST_H_

typedef enum kind kind;
typedef struct Node Node;
typedef struct LNodes LNodes;

enum kind { ARG0 = 1, ARG1, ASSN, BOOL, ELLI, NUM, NAME, STR, BOP, UOP, ELST, FLST,
            IFLST, SLST, BRAK, BRK, DO, ELIF, FBOD1, FBOD2, FC1, FC2, FDEF, FLD1, FLD2,
            FOR1, FOR2, FOR3, FUNC, GOTO, IF1, IF2, IF3, IF4, KY1, KY2, LABL, LFUNC,
            LOCL, LOCA, MEMB, REPEAT, RETN0, RETN1, TBL0, TBL1, WHILE };

struct Node {
  kind ntype;
  char *op, *val;
  int sz;
  Node **vec;
  LNodes *lnd;
};

struct LNodes {
  Node *nd;
  LNodes *next;
};

Node *add_Node(Node *vnd, Node *nd);
Node *mk_Node(kind ntype, int nvarg, ...);
Node *mk_Nlst(kind ntype, Node *nd);
Node *mk_Bop(kind ntype, char *op, Node *left, Node *right);
Node *mk_Uop(kind ntype, char *op, Node *right);
Node *mk_Bool(kind ntype, char *val);
Node *mk_Brk(kind ntype);
Node *mk_Elli(kind ntype);
Node *mk_Atom(kind ntype, char *val);
void dump_ast(Node *tree);
void free_Node(Node *tree);

#endif // _AST_H_
