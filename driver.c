//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#include "driver.h"
#include "parser.h"
#include "pp.h"
#include "scan.h"

module *new_mod_from_stdin() {
  module *mod = (module *) malloc(sizeof(module));
  mod->repl = 0;
  mod->src = stdin;
  mod->root = NULL;
  return mod;
}

module *new_mod_from_file(const char *filename) {
  module *mod = (module *) malloc(sizeof(module));
  mod->repl = 0;  
  mod->src = fopen(filename, "r");
  mod->root = NULL;
  return mod;
}

module *new_mod_from_string(char *src) {
  module *mod = (module *) malloc(sizeof(module));
  mod->repl = 1;  
  mod->src = fmemopen(src, strlen(src), "r");
  mod->root = NULL;
  return mod;
}

void delete_mod(module *mod) {
  if (mod->root != NULL) {
    free_Node(mod->root);
  }
  fclose(mod->src);
  free(mod);
}

int parse_mod(module *mod, char out) {
  int status;
  yyscan_t sc;
  YYSTYPE val;
  YYLTYPE loc;
  yypstate *ps = yypstate_new();
  struct sdata sd = { 0, 0, 0 };

  yylex_init(&sc);
  yyset_in(mod->src, sc);
  yyset_extra(&sd, sc);

  do {
    status = yypush_parse(ps, yylex(&val, &loc, sc), &val, &loc, sc, mod);
  } while(status == YYPUSH_MORE);

  if (status == 0) {
    pp(mod->root, out);
  }

  yypstate_delete(ps);
  yylex_destroy(sc);
  return status;
}

char *parse_mod_str(module *mod, int *err) {
  yyscan_t sc;
  YYSTYPE val;
  YYLTYPE loc;
  yypstate *ps = yypstate_new();
  struct sdata sd = { 0, 0, 0 };
  char *str = NULL;

  yylex_init(&sc);
  yyset_in(mod->src, sc);
  yyset_extra(&sd, sc);

  do {
    *err = yypush_parse(ps, yylex(&val, &loc, sc), &val, &loc, sc, mod);
  } while(*err == YYPUSH_MORE);

  if (*err == 0) {
    str = pp_str(mod->root);
  }

  yypstate_delete(ps);
  yylex_destroy(sc);
  return str;
}
