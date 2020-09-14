//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

%define api.pure full
%lex-param {void *scanner}
%parse-param {void *scanner}{module *mod}
%locations

/* Changes to a push parser */
%define api.push-pull push

%define parse.error detailed
%define parse.lac full

%{
#include <stdio.h>
#include "driver.h"
#include "parser.h"
#include "scan.h"

void yyerror(YYLTYPE *yylval, yyscan_t *locp, module *mod, char const *msg);
%}

%code requires {
#include "ast.h"
#include "driver.h"
#include "scan_utils.h"
}

%token TK_EOF 0 "EOF"
%token TK_AND 257 "and"
%token TK_BREAK "break"
%token TK_DO "do"
%token TK_ELSE "else"
%token TK_ELSEIF "elseif"
%token TK_END "end"
%token TK_FALSE "false"
%token TK_FOR "for"
%token TK_FUNCTION "function"
%token TK_GOTO "goto"
%token TK_IF "if"
%token TK_IN "in"
%token TK_LOCAL "local"
%token TK_NIL "nil"
%token TK_NOT "not"
%token TK_OR "or"
%token TK_REPEAT "repeat"
%token TK_RETURN "return"
%token TK_THEN "then"
%token TK_TRUE "true"
%token TK_UNTIL "until"
%token TK_WHILE "while"
%token TK_DOT "."
%token TK_CONCAT ".."
%token TK_ELLIPSIS "..."
%token TK_EQ "=="
%token TK_GE ">="
%token TK_LE "<="
%token TK_NE "~="
%token TK_GT ">"
%token TK_LT "<"
%token TK_ASSIGN "="
%token TK_ADD "+"
%token TK_SUB "-"
%token TK_MUL "*"
%token TK_DIV "/"
%token TK_MOD "%"
%token TK_CARAT "^"
%token TK_SEMI ";"
%token TK_COMMA ","
%token TK_HASH "#"
%token TK_COLON ":"
%token TK_DCOLON "::"
%token TK_LCB "{"
%token TK_RCB "}"
%token TK_LPAR "("
%token TK_RPAR ")"
%token TK_LB  "["
%token TK_RB "]"
%token <num> TK_NUMBER "number"
%token <id> TK_NAME "name"
%token <str> TK_STRING "string"
%token TK_BADCHAR
%token NOARG
%type <ast> args block bracket chunk elseifp elseifps exp explist field fields
%type <ast> fieldlist fname funcbody funcname functioncall functiondef ident key
%type <ast> label namelist parlist primary retstat stat statlist tableconstructor
%type <ast> var varlist

%union {
  char *id;
  char *num;
  char *str;
  Node *ast;
}

%nonassoc NOARG
%nonassoc "("
%left "or"
%left "and"
%left "<" ">" "<=" ">=" "~=" "=="
%right ".."
%left "+" "-"
%left "*" "/" "%"
%left "not" "#"
%right "^"

%start prog

%%

prog: chunk "EOF"   { mod->root = $1; }
    ;

chunk: statlist retstat OSC   { $$ = add_Node($1, $2); }
     | statlist               { $$ = $1; }
     ;

statlist: OSC                 { $$ = mk_Nlst(SLST, NULL); }
        | statlist stat OSC   { $$ = add_Node($1, $2); }
        ;

block: chunk   { $$ = $1; }
     ;

stat: varlist "=" explist                                    { $$ = mk_Node(ASSN, 2, $1, $3); }
    | functioncall %prec NOARG                               { $$ = $1; }
    | label                                                  { $$ = $1; }
    | "goto" ident                                           { $$ = mk_Node(GOTO, 1, $2); }
    | "do" block "end"                                       { $$ = mk_Node(DO, 1, $2); }
    | "while" exp "do" block "end"                           { $$ = mk_Node(WHILE, 2, $2, $4); }
    | "repeat" block "until" exp                             { $$ = mk_Node(REPEAT, 2, $2, $4); }
    | "if" exp "then" block "end"                            { $$ = mk_Node(IF1, 2, $2, $4); }
    | "if" exp "then" block "else" block "end"               { $$ = mk_Node(IF2, 3, $2, $4, $6); }
    | "if" exp "then" block elseifps "end"                   { $$ = mk_Node(IF3, 3, $2, $4, $5); }
    | "if" exp "then" block elseifps "else" block "end"      { $$ = mk_Node(IF4, 4, $2, $4, $5, $7); }
    | "for" ident "=" exp "," exp "do" block "end"           { $$ = mk_Node(FOR1, 4, $2, $4, $6, $8); }
    | "for" ident "=" exp "," exp "," exp "do" block "end"   { $$ = mk_Node(FOR2, 5, $2, $4, $6, $8, $10); }
    | "for" namelist "in" explist "do" block "end"           { $$ = mk_Node(FOR3, 3, $2, $4, $6); }
    | "function" funcname funcbody                           { $$ = mk_Node(FUNC, 2, $2, $3); }
    | "local" "function" ident funcbody                      { $$ = mk_Node(LFUNC, 2, $3, $4); }
    | "local" namelist                                       { $$ = mk_Node(LOCL, 1, $2); }
    | "local" namelist "=" explist                           { $$ = mk_Node(LOCA, 2, $2, $4); }
    ;

elseifp: "elseif" exp "then" block   { $$ = mk_Node(ELIF, 2, $2, $4); }
       ;

elseifps: elseifp            { $$ = mk_Nlst(IFLST, $1); }
        | elseifps elseifp   { $$ = add_Node($1, $2); }
        ;

retstat: "return"           { $$ = mk_Node(RETN0, 0); }
       | "return" explist   { $$ = mk_Node(RETN1, 1, $2); }
       | "break"            { $$ = mk_Brk(BRK); }
       ;

label: "::" ident "::"   {  $$ = mk_Node(LABL, 1, $2); }
     ;

funcname: fname             { $$ = $1; }
        | fname ":" ident   { $$ = mk_Node(MEMB, 2, $1, $3); }
        ;

varlist: var               { $$ = mk_Nlst(ELST, $1); }
       | varlist "," var   { $$ = add_Node($1, $3); }
       ;

var: ident              { $$ = $1; }
   | bracket key        { $$ = mk_Node(FC1, 2, $1, $2); }
   | functioncall key   { $$ = mk_Node(FC1, 2, $1, $2); }
   | var key            { $$ = mk_Node(FC1, 2, $1, $2); }
   ;

key: "[" exp "]"   { $$ = mk_Node(KY1, 1, $2); }
   | "." ident     { $$ = mk_Node(KY2, 1, $2); }
   ;

fname: ident             { $$ = mk_Nlst(FLST, $1); }
     | fname "." ident   { $$ = add_Node($1, $3); }
     ;

namelist: ident                { $$ = mk_Nlst(ELST, $1); }
        | namelist "," ident   { $$ = add_Node($1, $3); }
        ;

explist: exp               { $$ = mk_Nlst(ELST, $1); }
       | explist "," exp   { $$ = add_Node($1, $3); }
       ;

exp: primary                    { $$ = $1; }
   | var %prec NOARG            { $$ = $1; }
   | functioncall %prec NOARG   { $$ = $1; }
   | exp "or" exp               { $$ = mk_Bop(BOP, "or", $1, $3); }
   | exp "and" exp              { $$ = mk_Bop(BOP, "and", $1, $3); }
   | exp "<" exp                { $$ = mk_Bop(BOP, "<", $1, $3); }
   | exp ">" exp                { $$ = mk_Bop(BOP, ">", $1, $3); }
   | exp "<=" exp               { $$ = mk_Bop(BOP, "<=", $1, $3); }
   | exp ">=" exp               { $$ = mk_Bop(BOP, ">=", $1, $3); }
   | exp "~=" exp               { $$ = mk_Bop(BOP, "~=", $1, $3); }
   | exp "==" exp               { $$ = mk_Bop(BOP, "==", $1, $3); }
   | exp ".." exp               { $$ = mk_Bop(BOP, "..", $1, $3); }
   | exp "+" exp                { $$ = mk_Bop(BOP, "+", $1, $3); }
   | exp "-" exp                { $$ = mk_Bop(BOP, "-", $1, $3); }
   | exp "*" exp                { $$ = mk_Bop(BOP, "*", $1, $3); }
   | exp "/" exp                { $$ = mk_Bop(BOP, "/", $1, $3); }
   | exp "%" exp                { $$ = mk_Bop(BOP, "%", $1, $3); }
   | exp "^" exp                { $$ = mk_Bop(BOP, "^", $1, $3); }
   | "not" exp                  { $$ = mk_Uop(UOP, "not", $2); }
   | "#" exp                    { $$ = mk_Uop(UOP, "#", $2); }
   | "-" exp %prec "not"        { $$ = mk_Uop(UOP, "-", $2); }
   ;

primary: "false"               { $$ = mk_Bool(BOOL, "false"); }
       | "true"                { $$ = mk_Bool(BOOL, "true"); }
       | "nil"                 { $$ = mk_Bool(BOOL, "nil"); }
       | "number"              { $$ = mk_Atom(NUM, $1); }
       | "string"              { $$ = mk_Atom(STR, $1); }
       | "..."                 { $$ = mk_Elli(ELLI); }
       | functiondef           { $$ = $1; }
       | tableconstructor      { $$ = $1; }
       | bracket %prec NOARG   { $$ = $1; }
       ;

bracket: "(" exp ")"   { $$ = mk_Node(BRAK, 1, $2); }
       ;

functioncall: bracket args                  { $$ = mk_Node(FC1, 2, $1, $2); }
            | bracket ":" ident args        { $$ = mk_Node(FC2, 3, $1, $3, $4); }
            | var args                      { $$ = mk_Node(FC1, 2, $1, $2); }
            | var ":" ident args            { $$ = mk_Node(FC2, 3, $1, $3, $4); }
            | functioncall args             { $$ = mk_Node(FC1, 2, $1, $2); }
            | functioncall ":" ident args   { $$ = mk_Node(FC2, 3, $1, $3, $4); }
            ;

args: "(" ")"            { $$ = mk_Node(ARG0, 0); }
    | "(" explist ")"    { $$ = mk_Node(ARG1, 1, $2); }
    | tableconstructor   { $$ = $1; }
    | "string"           { $$ = mk_Atom(STR, $1); }
    ;

functiondef: "function" funcbody   { $$ = mk_Node(FDEF, 1, $2); }
           ;

funcbody: "(" ")" block "end"           { $$ = mk_Node(FBOD1, 1, $3); }
        | "(" parlist ")" block "end"   { $$ = mk_Node(FBOD2, 2, $2, $4); }
        ;

parlist: namelist             { $$ = $1; }
       | namelist "," "..."   { $$ = add_Node($1, mk_Elli(ELLI)); }
       | "..."                { $$ = mk_Elli(ELLI); }
       ;

tableconstructor: "{" "}"             { $$ = mk_Node(TBL0, 0); }
                | "{" fieldlist "}"   { $$ = mk_Node(TBL1, 1, $2); }
                ;

fieldlist: fields            { $$ = $1; }
         | fields fieldsep   { $$ = $1; }
         ;

fields: field                   { $$ = mk_Nlst(ELST, $1); }
      | fields fieldsep field   { $$ = add_Node($1, $3); }
      ;

field: "[" exp "]" "=" exp   { $$ = mk_Node(FLD1, 2, $2, $5); }
     | ident "=" exp         { $$ = mk_Node(FLD2, 2, $1, $3); }
     | exp                   { $$ = $1; }
     ;

fieldsep: ","   {}
        | ";"   {}
        ;

ident: "name"   { $$ = mk_Atom(NAME, $1); }
     ;

OSC :       {}
    | ";"   {}
    ;

%%

void yyerror(YYLTYPE *yylval, yyscan_t *locp, module *mod, char const *msg) {
    if (mod->repl) {
    } else {
        fprintf(stderr, "Error @(line, col)=(%d, %d): Unable to parse @msg[ %s ]\n",
                yylval->last_line, yylval->last_column, msg);
    }
}
