//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/token.c                  //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef TOKEN_C
#define TOKEN_C

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

void PrintTokenType(TTokenType type) {
  switch(type) {
    case TK_NA: printf("Token: TK_NA\n"); break;
    case TK_COMMA: printf("Token: TK_COMMA\n"); break; 
    case TK_EOF: printf("Token: TK_EOF\n"); break; 
    case TK_EOL: printf("Token: TK_EOL\n"); break; 
    case TK_ID: printf("Token: TK_ID\n"); break; 
    case TK_SEMICOLON: printf("Token: TK_SEMICOLON\n"); break;
    case TK_TERM: printf("Token: TK_TERM\n"); break; 

  // klicova slova
  
  case TK_AS: printf("Token: TK_AS\n"); break;
  case TK_ASC: printf("Token: TK_ASC\n"); break;
  case TK_DECLARE: printf("Token: TK_DECLARE\n"); break;
  case TK_DIM: printf("Token: TK_DIM\n"); break;
  case TK_DO: printf("Token: TK_DO\n"); break;
  case TK_DOUBLE: printf("Token: TK_DOUBLE\n"); break;
  case TK_ELSE: printf("Token: TK_ELSE\n"); break;
  case TK_END: printf("Token: TK_END\n"); break;
  case TK_CHR: printf("Token: TK_CHR\n"); break;
  case TK_FUNCTION: printf("Token: TK_FUNCTION\n"); break;
  case TK_IF: printf("Token: TK_IF\n"); break;
  case TK_INPUT: printf("Token: TK_INPUT\n"); break;
  case TK_INTEGER: printf("Token: TK_INTEGER\n"); break;
  case TK_LENGTH: printf("Token: TK_LENGTH\n"); break;
  case TK_LOOP: printf("Token: TK_LOOP\n"); break;
  case TK_PRINT: printf("Token: TK_PRINT\n"); break;
  case TK_RETURN: printf("Token: TK_RETURN\n"); break;
  case TK_SCOPE: printf("Token: TK_SCOPE\n"); break;
  case TK_STRING: printf("Token: TK_STRING\n"); break;
  case TK_SUBSTR: printf("Token: TK_SUBSTR\n"); break;
  case TK_THEN: printf("Token: TK_THEN\n"); break;
  case TK_WHILE: printf("Token: TK_WHILE\n"); break;
  
  // rezervovana klicova slova
  case TK_AND: printf("Token: TK_AND\n"); break;
  case TK_BOOLEAN: printf("Token: TK_BOOLEAN\n"); break;
  case TK_CONTINUE: printf("Token: TK_CONTINUE\n"); break;
  case TK_ELSEIF: printf("Token: TK_ELSEIF\n"); break;
  case TK_EXIT: printf("Token: TK_EXIT\n"); break;
  case TK_FALSE: printf("Token: TK_FALSE\n"); break;
  case TK_FOR: printf("Token: TK_FOR\n"); break;
  case TK_NEXT: printf("Token: TK_NEXT\n"); break;
  case TK_NOT: printf("Token: TK_NOT\n"); break;
  case TK_OR: printf("Token: TK_OR\n"); break;
  case TK_SHARED: printf("Token: TK_SHARED\n"); break;
  case TK_STATIC: printf("Token: TK_STATIC\n"); break;
  case TK_TRUE: printf("Token: TK_TRUE\n"); break;
  

    case TK_NUM_INTEGER: printf("Token: TK_NUM_INTEGER\n"); break;
    case TK_NUM_DOUBLE: printf("Token: TK_NUM_DOUBLE\n"); break;

    case TK_EQUAL: printf("Token: TK_EQUAL\n"); break; 
    case TK_NOT_EQUAL: printf("Token: TK_NOT_EQUAL\n"); break;
    case TK_LESS: printf("Token: TK_LESS\n"); break; 
    case TK_GREATER: printf("Token: TK_GREATER\n"); break; 
    case TK_LESS_EQUAL: printf("Token: TK_LESS_EQUAL\n"); break;
    case TK_GREATER_EQUAL: printf("Token: TK_GREATER_EQUAL\n"); break; 

    case TK_BRACKET_ROUND_LEFT: printf("Token: TK_BRACKET_ROUND_LEFT\n"); break;
    case TK_BRACKET_ROUND_RIGHT: printf("Token: TK_BRACKET_ROUND_RIGHT\n"); break;

    case TK_PLUS: printf("Token: TK_PLUS\n"); break; 
    case TK_MINUS: printf("Token: TK_MINUS\n"); break; 
    case TK_MUL: printf("Token: TK_MUL\n"); break; 
    case TK_DIV: printf("Token: TK_DIV\n"); break; 
    case TK_MOD: printf("Token: TK_MOD\n"); break; 
    case TK_DIV_INT: printf("Token: TK_DIV_INT\n"); break;
    default: printf("Token: ......\n"); break;
  } 
}


TToken* TokenInit() {
  TToken* tkn;
  if ((tkn = malloc(sizeof(TToken))) == NULL) {
    return NULL; 
  }
  tkn->type = TK_NA;
  tkn->string = NULL;
  return tkn;
}

void TokenDestroy(TToken* tkn) {
  if(tkn->string != NULL) {
    free(tkn->string);
  }
  free(tkn);
}

#endif

// Soubor: src/token.c
