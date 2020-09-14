//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#include <string.h>
#include "parser.h"
#include "scan_utils.h"
#include "scan.h"
#include "driver.h"


// If set tests the lexer
#ifdef YYLEX

int main(int argc, char **argv) {
  int tok;
  yyscan_t sc;  
  YYSTYPE val;
  YYLTYPE loc;
  struct sdata sd = { 0, 0, 0 };

  yylex_init(&sc);  

  FILE *infile = fopen(argv[1], "rt");
  yyset_in(infile, sc);
  yyset_extra(&sd, sc);

  while ((tok=yylex(&val, &loc, sc))) {  
     fprintf(stdout, "%03d %-13.13s: %s  loc: (%d,%d)\n", tok, TokenName(tok),
             yyget_text(sc), yyget_lineno(sc), yyget_column(sc));
  }
  yylex_destroy(sc);
  fclose(infile);
  return 0;
}

#else

int main(int argc, char **argv) {
  module *mod;
  int res;

  if (argc == 2) {
    mod = new_mod_from_stdin();
    if (strcmp(argv[1], "-sexp") == 0) {
      res = parse_mod(mod, 's');
    }
    else if (strcmp(argv[1], "-lua") == 0) {
      res = parse_mod(mod, 'l');
    }
    else {
      printf("Need to specify a flag: -sexp or -lua\n");
      res = -1;
    }
    delete_mod(mod);
    return res;
  }
  else if (argc == 3) {
    mod = new_mod_from_file(argv[2]);
    if (strcmp(argv[1], "-sexp") == 0) {
      res = parse_mod(mod, 's');
    }
    else if (strcmp(argv[1], "-lua") == 0) {
      res = parse_mod(mod, 'l');
    }
    else {
      printf("Need to specify a flag: -sexp or -lua\n");
      res = -1;
    }    
    delete_mod(mod);
    return res;
  }
  else {
    printf("Program requires a flag (-lua or -sexp) and input (file or stdin)\n");
  }
  return 0;
}

#endif
