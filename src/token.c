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

char* TokenTypes[] = {
  "TK_NA",
  "TK_COMMA",
  "TK_EOF", 
  "TK_EOL", 
  "TK_ID", 
  "TK_SEMICOLON",
  "TK_TERM",
  "TK_AS",
  "TK_ASC",
  "TK_DECLARE",
  "TK_DIM",
  "TK_DO",
  "TK_DOUBLE",
  "TK_ELSE",
  "TK_END",
  "TK_CHR",
  "TK_FUNCTION",
  "TK_IF",
  "TK_INPUT",
  "TK_INTEGER",
  "TK_LENGTH",
  "TK_LOOP",
  "TK_PRINT",
  "TK_RETURN",
  "TK_SCOPE",
  "TK_STRING",
  "TK_SUBSTR",
  "TK_THEN",
  "TK_WHILE",
  "TK_AND",
  "TK_BOOLEAN",
  "TK_CONTINUE",
  "TK_ELSEIF",
  "TK_EXIT",
  "TK_FALSE",
  "TK_FOR",
  "TK_NEXT",
  "TK_NOT",
  "TK_OR",
  "TK_SHARED",
  "TK_STATIC",
  "TK_TRUE",
  "TK_NUM_INTEGER",
  "TK_NUM_DOUBLE",
  "TK_NUM_STRING",
  "TK_EQUAL",
  "TK_NOT_EQUAL",
  "TK_LESS", 
  "TK_GREATER", 
  "TK_LESS_EQUAL", 
  "TK_GREATER_EQUAL", 
  "TK_BRACKET_ROUND_LEFT",
  "TK_BRACKET_ROUND_RIGHT", 
  "TK_PLUS",
  "TK_MINUS", 
  "TK_MUL", 
  "TK_DIV", 
  "TK_MOD", 
  "TK_DIV_INT"
};

void PrintTokenType(TTokenType type) {
  if(type < TK_NA && type > TK_DIV_INT) {
    printf("TokenType: ...\n");
  }
  else {
    printf("TokenType: %s\n", TokenTypes[type]);
  }
}

void PrintToken(TToken* tkn) {
  if(tkn->type < TK_NA && tkn->type > TK_DIV_INT) {
    printf("Token: ...\n");
  }
  else {
    printf("Token: Type: %-22s String: %s\n", TokenTypes[tkn->type], tkn->string );
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
  if(tkn != NULL) {
    if(tkn->string != NULL) {
      free(tkn->string);
    }
    free(tkn);
  }
}

#endif

// Soubor: src/token.c
