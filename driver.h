//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#ifndef _MODULE_H_
#define _MODULE_H_

#include <stdio.h>
#include "ast.h"

typedef struct {
  int repl;
  FILE  *src;
  Node  *root;
} module;

module *new_mod_from_stdin();
module *new_mod_from_file(const char *filename);
module *new_mod_from_string(char *src);
void delete_mod(module *mod);

int parse_mod(module *mod, char out);
char *parse_mod_str(module *mod, int *err);

#endif // _MODULE_H_
