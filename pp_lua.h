//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#ifndef _PP_LUA_H_
#define _PP_LUA_H_

#include "ast.h"

typedef struct opts opts;

struct opts {
  int ind;
  int lev;
};

void pp_lua(FILE *fl, Node *tree, opts *op);

#endif // _PP_LUA_H_
