//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#include <stdio.h>
#include "pp_lua.h"

void ws(FILE *fl, opts *op) {
  if (op->lev > 0) {
    fprintf(fl, "%*c", op->lev*op->ind, ' ');
  }
}

void nl(FILE *fl, opts *op) {
  fprintf(fl, "\n");
  ws(fl, op);
}

void indent(FILE *fl, opts *op) {
  op->lev = op->lev+1;
  fprintf(fl, "\n");
  ws(fl, op);
}

void dedent(FILE* fl, opts *op) {
  op->lev = op->lev-1;
  fprintf(fl, "\n");
  ws(fl, op);
}

void pp_lua(FILE *fl, Node *tree, opts *op) {
  LNodes *head = tree->lnd;
  switch(tree->ntype) {
    case ARG0:
      fprintf(fl, "()");
      break;
    case ARG1:
      fprintf(fl, "(");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, ")");
      break;      
    case ASSN:
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " = ");
      pp_lua(fl, tree->vec[1], op);
      break;
    case BRAK:
      fprintf(fl, "(");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, ")");
      break;      
    case DO:
      fprintf(fl, "do");
      indent(fl, op);
      pp_lua(fl, tree->vec[0], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;      
    case ELIF:
      fprintf(fl, "elseif ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " then");
      indent(fl, op);
      pp_lua(fl, tree->vec[1], op);
      break;      
    case FBOD1:
      fprintf(fl, "()");
      indent(fl, op);
      pp_lua(fl, tree->vec[0], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;
    case FBOD2:
      fprintf(fl, "(");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, ")");
      indent(fl, op);
      pp_lua(fl, tree->vec[1], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;      
    case FC1:
      pp_lua(fl, tree->vec[0], op);
      pp_lua(fl, tree->vec[1], op);
      break;
    case FC2:
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, ":");
      pp_lua(fl, tree->vec[1], op);
      pp_lua(fl, tree->vec[2], op);
      break;
    case FDEF:
      fprintf(fl, "function ");
      pp_lua(fl, tree->vec[0], op);
      break;
    case FLD1:
      fprintf(fl, "[");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, "] = ");
      pp_lua(fl, tree->vec[1], op);
      break;
    case FLD2:
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " = ");
      pp_lua(fl, tree->vec[1], op);
      break;      
    case FOR1:
      fprintf(fl, "for ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " = ");
      pp_lua(fl, tree->vec[1], op);
      fprintf(fl, ", ");
      pp_lua(fl, tree->vec[2], op);      
      fprintf(fl, " do");
      indent(fl, op);
      pp_lua(fl, tree->vec[3], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;      
    case FOR2:
      fprintf(fl, "for ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " = ");
      pp_lua(fl, tree->vec[1], op);
      fprintf(fl, ", ");
      pp_lua(fl, tree->vec[2], op);
      fprintf(fl, ", ");
      pp_lua(fl, tree->vec[3], op);            
      fprintf(fl, " do");
      indent(fl, op);
      pp_lua(fl, tree->vec[4], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;      
    case FOR3:
      fprintf(fl, "for ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " in ");
      pp_lua(fl, tree->vec[1], op);
      fprintf(fl, " do");
      indent(fl, op);
      pp_lua(fl, tree->vec[2], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;      
    case FUNC:
      fprintf(fl, "function ");
      pp_lua(fl, tree->vec[0], op);
      pp_lua(fl, tree->vec[1], op);
      break;
    case GOTO:
      fprintf(fl, "goto ");
      pp_lua(fl, tree->vec[0], op);
      break;
    case IF1:
      fprintf(fl, "if ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " then");
      indent(fl, op);
      pp_lua(fl, tree->vec[1], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;            
    case IF2:
      fprintf(fl, "if ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " then");
      indent(fl, op);
      pp_lua(fl, tree->vec[1], op);
      dedent(fl, op);
      fprintf(fl, "else");
      indent(fl, op);
      pp_lua(fl, tree->vec[2], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;            
    case IF3:
      fprintf(fl, "if ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " then");
      indent(fl, op);
      pp_lua(fl, tree->vec[1], op);
      dedent(fl, op);
      pp_lua(fl, tree->vec[2], op);      
      dedent(fl, op);
      fprintf(fl, "end");
      break;            
    case IF4:
      fprintf(fl, "if ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " then");
      indent(fl, op);
      pp_lua(fl, tree->vec[1], op);
      dedent(fl, op);
      pp_lua(fl, tree->vec[2], op);      
      dedent(fl, op);
      fprintf(fl, "else");
      indent(fl, op);
      pp_lua(fl, tree->vec[3], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;            
    case KY1:      
      fprintf(fl, "[");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, "]");
      break;
    case KY2:
      fprintf(fl, ".");
      pp_lua(fl, tree->vec[0], op);
      break;
    case LABL:
      fprintf(fl, "::");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, "::");
      break;
    case LFUNC:
      fprintf(fl, "local function ");
      pp_lua(fl, tree->vec[0], op);
      pp_lua(fl, tree->vec[1], op);
      break;            
    case LOCA:
      fprintf(fl, "local ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " = ");
      pp_lua(fl, tree->vec[1], op);
      break;            
    case LOCL:
      fprintf(fl, "local ");
      pp_lua(fl, tree->vec[0], op);
      break;
    case MEMB:
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, ":");
      pp_lua(fl, tree->vec[1], op);
      break;
    case REPEAT:
      fprintf(fl, "repeat ");
      indent(fl, op);
      pp_lua(fl, tree->vec[0], op);
      dedent(fl, op);
      fprintf(fl, "until ");
      pp_lua(fl, tree->vec[1], op);
      break;            
    case RETN0:
      fprintf(fl, "return");
      break;
    case RETN1:
      fprintf(fl, "return ");
      pp_lua(fl, tree->vec[0], op);
      break;
    case TBL0:
      fprintf(fl, "{}");
      break;
    case TBL1:
      fprintf(fl, "{");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, "}");        
      break;      
    case WHILE:
      fprintf(fl, "while ");
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " do");
      indent(fl, op);
      pp_lua(fl, tree->vec[1], op);
      dedent(fl, op);
      fprintf(fl, "end");
      break;            
    case ELST:
      while (head != NULL) {
        pp_lua(fl, head->nd, op);
        if (head->next != NULL) {
          fprintf(fl, ", "); 
        }
        head = head->next;
      }
      break;      
    case FLST:
      while (head != NULL) {
        pp_lua(fl, head->nd, op);
        if (head->next != NULL) {
          fprintf(fl, "."); 
        }        
        head = head->next;
      }
      break;            
    case IFLST:
      while (head != NULL) {
        pp_lua(fl, head->nd, op);
        if (head->next != NULL) {
          dedent(fl, op);
        }
        head = head->next;
      }
      break;            
    case SLST:
      while (head != NULL) {
        pp_lua(fl, head->nd, op);
        if (head->next != NULL) {
          nl(fl, op);
        }
        head = head->next;
      }
      break;      
    case BOP:
      pp_lua(fl, tree->vec[0], op);
      fprintf(fl, " %s ", tree->op);      
      pp_lua(fl, tree->vec[1], op);
      break;
    case UOP:
      fprintf(fl, "%s ", tree->op);
      pp_lua(fl, tree->vec[0], op);
      break;
    case BRK:
      fprintf(fl, "break");
      break;            
    case BOOL:
    case ELLI:
    case NAME:
    case NUM:
    case STR:
      fprintf(fl, "%s", tree->val);
      break;
    default:
      fprintf(fl, "pp_lua: Serious problem!\n");
      break;
  }
}
