/*
 The MIT License

 Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>
*/

%{
#include "parser.h"
#include <stdio.h>

#define YY_USER_ACTION \
    { \
       yylloc->first_line = yylloc->last_line; \
       yylloc->first_column = yylloc->last_column; \
       yylloc->last_line = yylineno; \
       yylloc->last_column = yycolumn; \
       yycolumn += yyleng; \
    }
%}

%option reentrant bison-bridge bison-locations
%option yylineno noyywrap nounput
%option extra-type="struct sdata *"
                        
ws     [ \t\v\a]+
name   [_a-zA-Z][_a-zA-Z0-9]*
int    [0-9]+
hex    0[Xx][0-9a-fA-F]+                        
exp    [Ee][+-]?{int}
dec    {int}|{int}[.]{int}|[.]{int}|{int}[.]                        
flt    {dec}{exp}?
num    {hex}|{flt}                        
eol    [\n\r]
sep    =*

%x LSTRING
%x COMMENT
%x LCOMMENT
%x STRINGQ
%x STRINGA

%%
%{
  struct sdata *sdt = yyextra;
%}

^#!.*          {  if (yylineno==1) {
                  }  /* ignore it */
                  else {  
		            fprintf(yyout,"Misplaced: %s\n", yytext);
                  }
               }
{name}         { yylval->id = yytext; return checkKw(yytext); }
{num}          { yylval->num = yytext; return TK_NUMBER; }
"--["{sep}"["  { sdt->ncol=yycolumn; yymore(); sdt->lcmi=yyleng; sdt->lcml=0; BEGIN(LCOMMENT); }
"["{sep}"["    { sdt->ncol=yycolumn; yymore(); sdt->lcmi=yyleng; sdt->lcml=0; BEGIN(LSTRING); }
"--"           { yymore(); BEGIN(COMMENT); }
{ws}           {}
\"             { sdt->ncol=yycolumn; yymore(); BEGIN(STRINGQ); }
'              { sdt->ncol=yycolumn; yymore(); BEGIN(STRINGA); }
"..."          { return TK_ELLIPSIS; }
".."           { return TK_CONCAT; }
"."            { return TK_DOT; }
"=="           { return TK_EQ; }
">="           { return TK_GE; }
"<="           { return TK_LE; }
"~="           { return TK_NE; }
">"            { return TK_GT; }
"<"            { return TK_LT; }
"="            { return TK_ASSIGN; }
"+"            { return TK_ADD; }
"-"            { return TK_SUB; }
"*"            { return TK_MUL; }
"/"            { return TK_DIV; }
"%"            { return TK_MOD; }
"^"            { return TK_CARAT; }
";"            { return TK_SEMI; }
","            { return TK_COMMA; }
"#"            { return TK_HASH; }
"::"           { return TK_DCOLON; }
":"            { return TK_COLON; }
"{"            { return TK_LCB; }
"}"            { return TK_RCB; }
"("            { return TK_LPAR; }
")"            { return TK_RPAR; }
"["            { return TK_LB; }
"]"            { return TK_RB; }  
{eol}          { yycolumn = 0; }
<<EOF>>        { return TK_EOF; }
.              { fprintf(stderr, "Error @(line, col)=(%d, %d): Unable to match @token[ %s ]\n",
                         yylineno, yycolumn, &yytext[0]); }


<STRINGQ>
{
  \"        { yycolumn=sdt->ncol+1; BEGIN(0); yylval->str = yytext; return TK_STRING; }
  \\\"      { sdt->ncol=sdt->ncol+2; yymore(); }
  "|"       { sdt->ncol=sdt->ncol+1; yymore(); }
  \\\n      { yycolumn=0; sdt->ncol=0; yymore(); }
  \\\\      { sdt->ncol=sdt->ncol+2; yymore(); }
  [\n|\r]   { STR1 }
  .         { sdt->ncol=sdt->ncol+1; yymore(); }
}

<STRINGA>
{
  '         { yycolumn=sdt->ncol+1; BEGIN(0); yylval->str = yytext; return TK_STRING; }
  \\'       { sdt->ncol=sdt->ncol+2; yymore(); }
  \\\\      { sdt->ncol=sdt->ncol+2; yymore(); }
  [\n|\r]   { STR2 }
  .         { sdt->ncol=sdt->ncol+1; yymore(); }
}

<COMMENT>
{
  {eol}   { yycolumn=0; BEGIN(0); }
  .       { yymore(); }
}

<LCOMMENT>
{
  "]]"       { LCOM1 }
  "]"{sep}   { LCOM2 }
  {eol}      { yycolumn=0; sdt->ncol=0; sdt->lcml=sdt->lcml+1; yymore(); }
  .          { sdt->ncol=sdt->ncol+1; sdt->lcml=sdt->lcml+1; yymore(); }
}

<LSTRING>
{
  "]]"        { LSTR1 }
  "]"{sep}    { LSTR2 }
  {eol}       { yycolumn=0; sdt->ncol=0; sdt->lcml=sdt->lcml+1; yymore(); }
  .           { sdt->ncol=sdt->ncol+1; sdt->lcml=sdt->lcml+1; yymore(); }
}
