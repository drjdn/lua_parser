//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ast.h"
#include "parser.h"


LNodes *mk_LNodes(Node *expr) {
  LNodes *v = (LNodes *) malloc(sizeof(LNodes));
  v->nd = expr;
  v->next = NULL;
  return v;
}

LNodes *add_LNodes(LNodes *head, Node *data) {
  LNodes *cursor = head;
  while(cursor->next != NULL) {
    cursor = cursor->next;
  }
  LNodes* new_node =  mk_LNodes(data);
  cursor->next = new_node; 
  return head;
}

Node *add_Node(Node *vnd, Node *nd) {
  if (vnd->lnd == NULL) {
    vnd->lnd = mk_LNodes(nd);
  } else {
    vnd->lnd = add_LNodes(vnd->lnd, nd);
  }
  return vnd;
}

Node *mk_Nlst(kind ntype, Node *nd) {
  Node *e = (Node *) malloc(sizeof(Node));
  e->ntype = ntype;
  if (nd == NULL) {
    e->lnd = NULL;
  } else {
    e->lnd = mk_LNodes(nd);
  }
  return e;
}

Node *mk_Node(kind ntype, int nvarg, ...) {
    Node *e = (Node *) malloc(sizeof(Node));
    e->ntype = ntype;
    e->sz = nvarg;
    if (e->sz == 0) {
      return e;
    }
    va_list varg;
    va_start(varg, nvarg);
    e->vec = (Node **) malloc(nvarg * sizeof(Node));
    for(int i = 0; i < nvarg; i++) {
      e->vec[i] = va_arg(varg, Node *);
    }
    va_end(varg);
    return e;
}

Node *mk_Uop(kind ntype, char *op, Node *right) {
  Node *e = (Node *) malloc(sizeof(Node));
  e->ntype = ntype;
  e->op = op;
  e->sz = 1;
  e->vec = (Node **) malloc((e->sz) * sizeof(Node));
  e->vec[0] = right;
  return e;
}

Node *mk_Bop(kind ntype, char *op, Node *left, Node *right) {
  Node *e = (Node *) malloc(sizeof(Node));
  e->ntype = ntype;
  e->op = op;
  e->sz = 2;
  e->vec = (Node **) malloc((e->sz) * sizeof(Node));  
  e->vec[0] = left;
  e->vec[1] = right;
  return e;
}

Node *mk_Atom(kind ntype, char *val) {
  Node *e = (Node *) malloc(sizeof(Node));
  e->val = (char *) malloc(strlen((char *) val)+1);
  e->ntype = ntype;
  e->sz = 0;
  strcpy(e->val, (char *) val);
  return e;
}

Node *mk_Bool(kind ntype, char *val) {
  Node *e = (Node *) malloc(sizeof(Node));
  e->ntype = ntype;
  e->val = val;
  e->sz = 0;
  return e;
}

Node *mk_Brk(kind ntype) {
  Node *e = (Node *) malloc(sizeof(Node));
  e->ntype = ntype;
  e->val = "break";
  e->sz = 0;
  return e;
}

Node *mk_Elli(kind ntype) {
  Node *e = (Node *) malloc(sizeof(Node));
  e->ntype = ntype;
  e->val = "...";
  e->sz = 0;
  return e;
}

void free_Node(Node *tree) {
  int vlen = tree->sz;
  LNodes *head;  
  switch(tree->ntype) {
    case ARG1:
    case ASSN:
    case BRAK:
    case DO:
    case ELIF:
    case FBOD1:
    case FBOD2:      
    case FC1:
    case FC2:
    case FDEF:
    case FLD1:
    case FLD2:
    case FOR1:
    case FOR2:
    case FOR3:
    case FUNC:
    case GOTO:
    case IF1:
    case IF2:
    case IF3:
    case IF4:
    case KY1:
    case KY2:            
    case LABL:
    case LFUNC:
    case LOCA:
    case LOCL:
    case MEMB:
    case REPEAT:
    case RETN1:
    case TBL1:
    case WHILE:
    case BOP:
    case UOP:
      for(int i = 0; i < vlen; ++i) {
        if (tree->vec[i] != NULL) {
          free_Node(tree->vec[i]);
        }
      }
      free(tree->vec);
      break;
    case ELST:
    case FLST:
    case IFLST:
    case SLST:
      while (tree->lnd != NULL) {
        head = tree->lnd;
        free_Node(head->nd);
        tree->lnd = head->next;
        free(head);
      }      
      break;
    case NAME:
    case NUM:
    case STR:
      free(tree->val);
      break;
    case ARG0:
    case BRK:
    case ELLI:
    case BOOL:
    case RETN0:
    case TBL0:
      break;
  }
  free(tree);
}
