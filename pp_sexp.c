//The MIT License
//
//Copyright (c) 2020 Jason D. Nielsen <drjdnielsen@gmail.com>

#include <stdio.h>
#include "pp_sexp.h"

void pp_sexp(FILE *fl, Node *tree) {
  LNodes *head = tree->lnd;  
  switch(tree->ntype) {
    case ARG0:
      fprintf(fl, "(args)");
      break;
    case ARG1:
      fprintf(fl, "(args ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");        
      break;      
    case ASSN:
      fprintf(fl, "(= ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;
    case BRAK:
      fprintf(fl, "(bracket ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");
      break;      
    case DO:
      fprintf(fl, "(do ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");
      break;      
    case ELIF:
      fprintf(fl, "(elseif ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " (then ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, "))");
      break;      
    case FBOD1:
      fprintf(fl, "(fbody ");
      fprintf(fl, "(fargs)");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");                      
      break;
    case FBOD2:
      fprintf(fl, "(fbody ");
      fprintf(fl, "(fargs ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");              
      break;      
    case FC1:
      fprintf(fl, "(fcall ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;
    case FC2:
      fprintf(fl, "(fcall (: ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ") ");
      pp_sexp(fl, tree->vec[2]);
      fprintf(fl, ")");
      break;
    case FDEF:
      fprintf(fl, "(func ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");            
      break;
    case FLD1:
    case FLD2:
      fprintf(fl, "(field ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;      
    case FOR1:
      fprintf(fl, "(for (= ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[2]);      
      fprintf(fl, ")");
      fprintf(fl, "(do ");
      pp_sexp(fl, tree->vec[3]);
      fprintf(fl, "))");
      break;      
    case FOR2:
      fprintf(fl, "(for (= ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[2]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[3]);            
      fprintf(fl, ")");
      fprintf(fl, "(do ");
      pp_sexp(fl, tree->vec[4]);
      fprintf(fl, "))");
      break;
    case FOR3:
      fprintf(fl, "(for (in ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      fprintf(fl, "(do ");
      pp_sexp(fl, tree->vec[2]);
      fprintf(fl, "))");
      break;      
    case FUNC:
      fprintf(fl, "(func ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;
    case GOTO:
      fprintf(fl, "(goto ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");
      break;
    case IF1:
      fprintf(fl, "(if ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;            
    case IF2:
      fprintf(fl, "(if ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, "(else ");
      pp_sexp(fl, tree->vec[2]);
      fprintf(fl, "))");
      break;            
    case IF3:
      fprintf(fl, "(if ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[2]);
      fprintf(fl, ")");
      break;            
    case IF4:
      fprintf(fl, "(if ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[2]);
      fprintf(fl, "(else ");
      pp_sexp(fl, tree->vec[3]);
      fprintf(fl, "))");
      break;            
    case KY1:
    case KY2:
      fprintf(fl, "(key ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");
      break;            
    case LABL:
      fprintf(fl, "(label ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");
      break;
    case LFUNC:
      fprintf(fl, "(lfunc ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;            
    case LOCA:
      fprintf(fl, "(local (= ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, "))");
      break;            
    case LOCL:
      fprintf(fl, "(local ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");
      break;
    case MEMB:
      fprintf(fl, ": ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");      
      break;
    case REPEAT:
      fprintf(fl, "(repeat ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;
    case RETN0:
      fprintf(fl, "(return)");
      break;
    case RETN1:
      fprintf(fl, "(return ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");        
      break;
    case TBL0:
      fprintf(fl, "(table)");
      break;
    case TBL1:
      fprintf(fl, "(table ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");        
      break;      
    case WHILE:
      fprintf(fl, "(while ");
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;            
    case ELST:
    case FLST:
    case IFLST:
      while (head != NULL) {
        pp_sexp(fl, head->nd);
        if (head->next != NULL) {
          fprintf(fl, " ");          
        }        
        head = head->next;
      }
      break;
    case SLST:
      while (head != NULL) {
        pp_sexp(fl, head->nd);
        if (head->next != NULL) {
          fprintf(fl, "\n");          
        }
        head = head->next;
      }
      break;      
    case BOP:
      fprintf(fl, "(%s ", tree->op);
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, " ");
      pp_sexp(fl, tree->vec[1]);
      fprintf(fl, ")");
      break;
    case UOP:
      fprintf(fl, "(%s ", tree->op);
      pp_sexp(fl, tree->vec[0]);
      fprintf(fl, ")");
      break;
    case BRK:
      fprintf(fl, "(break)");
      break;            
    case BOOL:
    case ELLI:
    case NAME:
    case NUM:
    case STR:
      fprintf(fl, "%s", tree->val);
      break;
    default:
      fprintf(fl, "pp_sexp: Serious problem!\n");
      break;
  }
}
