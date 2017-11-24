//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/conversion.c             //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef CONVERSION_C
#define CONVERSION_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "conversion.h"
#include "token.h"
#include "ctype.h"

TTokenType keywords[] = {
  TK_AS,
  TK_ASC,
  TK_DECLARE,
  TK_DIM,
  TK_DO,
  TK_DOUBLE,
  TK_ELSE,
  TK_END,
  TK_CHR,
  TK_FUNCTION,
  TK_IF,
  TK_INPUT,
  TK_INTEGER,
  TK_LENGTH,
  TK_LOOP,
  TK_PRINT,
  TK_RETURN,
  TK_SCOPE,
  TK_STRING,
  TK_SUBSTR,
  TK_THEN,
  TK_WHILE,
  TK_AND,
  TK_BOOLEAN,
  TK_CONTINUE,
  TK_ELSEIF,
  TK_EXIT,
  TK_FALSE,
  TK_FOR,
  TK_NEXT,
  TK_NOT,
  TK_OR,
  TK_SHARED,
  TK_STATIC,
  TK_TRUE,
  TK_MOD
};

void Convert(TToken *token) {
  switch(token->type) {
    case TK_NUM_INTEGER:
      token->data.integerValue = strtol(token->string, NULL, 10);
      break;
    case TK_NUM_DOUBLE:
      token->data.doubleValue = strtod(token->string, NULL);
      break;
    case TK_ID:
      KeywordFilter(token);
      break;
    default:
      break;
  }
}

void KeywordFilter(TToken *token) {
  int len = sizeof(keywords) / sizeof(keywords[0]);

  char* str = token->string;
  char string[15] = "";
  //strcpy(string, str);

  for (unsigned int i = 0; i < strlen(str); i++) {
    string[i+3] = toupper(str[i]);
  }

  string[0] = 'T';
  string[1] = 'K';
  string[2] = '_';

  for (int i = 0; i < len; i++) {
    if (strcmp(string, TokenTypes[keywords[i]]) == 0) {
      token->type = keywords[i];
      break;
    }
  }
}

#endif

// Soubor: src/conversion.c
