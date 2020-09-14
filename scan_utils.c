//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#include <stdio.h>
#include <string.h>
#include "parser.h"

int checkKw(char*s) {
#define CHECK(S,T) if(!strcmp(#S,s)) return TK_##T;
  switch(*s) {  
    case 'a':
      CHECK(and,AND);
      break;
    case 'b':
      CHECK(break,BREAK);
      break;
    case 'd':
      CHECK(do,DO);
      break;
    case 'e':
      CHECK(elseif,ELSEIF);
      CHECK(else,ELSE);
      CHECK(end,END);
      break;
    case 'f':      
      CHECK(false,FALSE);
      CHECK(for,FOR);
      CHECK(function,FUNCTION);
      break;
    case 'g':      
      CHECK(goto,GOTO);
      break;
    case 'i':
      CHECK(if,IF);
      CHECK(in,IN);
      break;
    case 'l':
      CHECK(local,LOCAL);
      break;
    case 'n':
      CHECK(nil,NIL);
      CHECK(not,NOT);
      break;
    case 'o':
      CHECK(or,OR);
      break;
    case 'r':
      CHECK(repeat,REPEAT);
      CHECK(return,RETURN);
      break;
    case 't':
      CHECK(then,THEN);
      CHECK(true,TRUE);
      break;
    case 'u':
      CHECK(until,UNTIL);
      break;
    case 'w':
      CHECK(while,WHILE);
      break;
  }
  return TK_NAME;
#undef CHECK
}

char *TokenName(int t) {
  static char buffer[80];
  if( t < 0 || t == 256 ) return "<ERROR>";
  if( t == 0 ) return "EOF";
  if( t < 257 ) {  
    sprintf( buffer, "CHAR %c", (unsigned char)(unsigned int)t );
    return (char*)buffer;
  }
  switch(t) {  
    case TK_AND:        return "AND";
    case TK_BREAK:      return "BREAK";
    case TK_DO:         return "DO";
    case TK_ELSE:       return "ELSE";
    case TK_ELSEIF:     return "ELSEIF";
    case TK_END:        return "END";
    case TK_FALSE:      return "FALSE";
    case TK_FOR:        return "FOR";
    case TK_FUNCTION:   return "FUNCTION";
    case TK_GOTO:       return "GOTO";
    case TK_IF:         return "IF";
    case TK_IN:         return "IN";
    case TK_LOCAL:      return "LOCAL";
    case TK_NIL:        return "NIL";
    case TK_NOT:        return "NOT";
    case TK_OR:         return "OR";
    case TK_REPEAT:     return "REPEAT";
    case TK_RETURN:     return "RETURN";
    case TK_THEN:       return "THEN";
    case TK_TRUE:       return "TRUE";
    case TK_UNTIL:      return "UNTIL";
    case TK_WHILE:      return "WHILE";
    case TK_DOT:        return "DOT";
    case TK_CONCAT:     return "CONCAT";
    case TK_ELLIPSIS:   return "ELLIPSIS";
    case TK_EQ:         return "EQ";
    case TK_GE:         return "GE";
    case TK_LE:         return "LE";
    case TK_NE:         return "NE";
    case TK_GT:         return "GT";
    case TK_LT:         return "LT";
    case TK_ASSIGN:     return "ASSIGN";
    case TK_ADD:        return "ADD";
    case TK_SUB:        return "SUB";
    case TK_MUL:        return "MUL";
    case TK_DIV:        return "DIV";
    case TK_MOD:        return "MOD";
    case TK_CARAT:      return "CARAT";
    case TK_SEMI:       return "SEMI";
    case TK_COMMA:      return "COMMA";
    case TK_HASH:       return "HASH";
    case TK_DCOLON:     return "DCOLON";
    case TK_COLON:      return "COLON";
    case TK_LCB:        return "LCB";
    case TK_RCB:        return "RCB";
    case TK_LPAR:       return "LPAR";
    case TK_RPAR:       return "RPAR";
    case TK_LB:         return "LB";
    case TK_RB:         return "RB";
    case TK_NUMBER:     return "NUMBER";
    case TK_NAME:       return "NAME";
    case TK_STRING:     return "STRING";
    case TK_BADCHAR:    return "BADCHAR";
    default: break;
  }
  sprintf( buffer, "<? %d>", t );
  return buffer;
}
