//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#ifndef _SCAN_UTILS_H_
#define _SCAN_UTILS_H_


struct sdata {
  int lcmi;
  int lcml;
  int ncol;
};

int checkKw(char *s);
char *TokenName(int t);

// Scanner actions, to keep scan.l logic clear
#define STR1 \
fprintf(yyout,"Unterminated \"string\".\n"); yycolumn=0; sdt->ncol=0; BEGIN(0); yylval->str = yytext; return TK_STRING;  

#define STR2 \
fprintf(yyout,"Unterminated 'string'.\n"); yycolumn=0; sdt->ncol=0; BEGIN(0); yylval->str = yytext; return TK_STRING;  

#define LCOM1                  \
sdt->ncol=sdt->ncol+2;         \
if ((sdt->lcmi-2) == 2) {      \
 yycolumn = sdt->ncol;         \
 BEGIN(0);                     \
} else {                       \
 sdt->lcml=yyleng-sdt->lcmi;   \
 yymore();                     \
}

#define LCOM2                                        \
sdt->ncol=sdt->ncol+sdt->lcmi-3;                     \
if ((sdt->lcmi-3)==(yyleng-sdt->lcml-sdt->lcmi)) {   \
 char c = input(yyscanner);                          \
 strncat(yytext, &c, 1);                             \
 sdt->ncol=sdt->ncol+1;                              \
 if (c == ']') {                                     \
   yycolumn = sdt->ncol;                             \
   BEGIN(0);                                         \
 } else {                                            \
   yymore();                                         \
 }                                                   \
} else {                                             \
 sdt->lcml=yyleng-sdt->lcmi;                         \
 yymore();                                           \
}

#define LSTR1                  \
sdt->ncol=sdt->ncol+2;         \
if (sdt->lcmi == 2) {          \
 yycolumn = sdt->ncol;         \
 BEGIN(0);                     \
 yylval->str = yytext;         \
 return TK_STRING;             \
} else {                       \
 sdt->lcml=yyleng-sdt->lcmi;   \
 yymore();                     \
}

#define LSTR2                                        \
sdt->ncol=sdt->ncol+sdt->lcmi-1;                     \
if ((sdt->lcmi-1)==(yyleng-sdt->lcml-sdt->lcmi)) {   \
 char c = input(yyscanner);                          \
 strncat(yytext, &c, 1);                             \
 sdt->ncol=sdt->ncol+1;                              \
 if (c == ']') {                                     \
   yycolumn = sdt->ncol;                             \
   BEGIN(0);                                         \
   yylval->str = yytext;                             \
   return TK_STRING;                                 \
 } else {                                            \
   yymore();                                         \
 }                                                   \
} else {                                             \
 sdt->lcml=yyleng-sdt->lcmi;                         \
 yymore();                                           \
}

#endif
