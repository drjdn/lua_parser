//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "pp_lua.h"
#include "pp_sexp.h"

void pp(Node *tree, char out) {
  if (out == 's') {
    pp_sexp(stdout, tree);
    printf("\n");    
  }
  else if (out == 'l') {
    opts ops;
    ops.ind = 3;
    ops.lev = 0;
    pp_lua(stdout, tree, &ops);
    printf("\n");
  }
  else {
    printf("pp: This should not happen!\n");
  }
}

char *pp_str(Node *tree) {
  FILE *tmp = tmpfile();
  opts ops;
  long len;
  char *buf = NULL;
  ops.ind = 3;
  ops.lev = 0;
  pp_lua(tmp, tree, &ops);  
  fprintf(tmp, "\n");
  fseek(tmp, 0, SEEK_END);
  len = ftell(tmp);
  fseek(tmp, 0, SEEK_SET);
  buf = malloc((len+1) * sizeof(char));
  if (buf) {
    fread(buf, 1, len, tmp);
  }
  buf[len] = '\0';
  fclose(tmp);
  return buf;
}
