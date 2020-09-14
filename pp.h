//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#ifndef _PP_H_
#define _PP_H_

#include "ast.h"

void pp(Node *tree, char out);
char *pp_str(Node *tree);

#endif // _PP_H_
