//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/parser.c                 //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////


#ifndef PARSER_C
#define PARSER_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "token.h"
#include "error.h"


int repeatLastChar = 0;
TToken* readToken;
int readLastChar;
unsigned long int readLineNumber = 1;

int CanBeIgnored(char c) {
  char* ignor = "\n\t\r \v";
  int i = 0;
  while(ignor[i]!=0) {
    if(ignor[i]==c) 
      return 1;
    i++;
  }
  return 0;
}

TToken* GetNextToken() {

  if( (readToken=malloc(sizeof(TToken)))==NULL ) {
    // ERR_INTERNAL
    CallError(ERR_INTERNAL);
    return NULL;
  }
  readToken->type = TK_NA;

  do {
    // Znovuzpracovani posledniho znaku nebo nacteni noveho.
    if(repeatLastChar) {
      repeatLastChar = 0;
    }
    else {
      readLastChar=getchar();
    }


    if( readLastChar=='\'' ) {
      State_InlineComment(); 
    }
    else if( readLastChar=='/' ) {
      State_DivOrMultilineComment();
      if(readToken->type==TK_DIV) {
        break;
      }
    }
    else if( isalpha(readLastChar) || readLastChar=='_' ) {
      //StringAdd(s,readLastChar);
      State_ID();
      break;
    }
    else if( isdigit(readLastChar) ) {
      //StringAdd(s,readLastChar);
      State_Number();
      break;
    }
    else if( readLastChar=='!' ) {
      State_ExMark();
      break;
    }
    else if( readLastChar=='+' ) {
      readToken->type = TK_PLUS;
      break;
    }
    else if( readLastChar=='-' ) {
      readToken->type = TK_MINUS;
      break;
    }
    else if( readLastChar=='*' ) {
      readToken->type = TK_MUL;
      break;
    }
    else if( readLastChar=='\\' ) {
      readToken->type = TK_DIV_INT;
      break;
    }
    else if( readLastChar==';' ) {
      readToken->type = TK_SEMICOLON;
      break;
    }
    else if( readLastChar=='\n' ) {
      readToken->type = TK_EOL;
      readLineNumber++;
      break;
    }
    else if( readLastChar==EOF ) {
      readToken->type = TK_EOF;
      break;
    }
    else if( readLastChar=='=' ) {
      readToken->type = TK_EQUAL;
      break;
    }
    else if( readLastChar=='>' ) {
      State_GreaterThan();
      break;
    }
    else if( readLastChar=='<' ) {
      State_LessThan();
      break;
    }
    else if( readLastChar=='(' ) {
      readToken->type = TK_BRACKET_ROUND_LEFT;
      break;
    }
    else if( readLastChar==')' ) {
      readToken->type = TK_BRACKET_ROUND_RIGHT;
      break;
    }
    else if( CanBeIgnored(readLastChar) ) { ; }
    else {
      // ERR_LEX
      CallError(ERR_LEX);
      break;
    }
  } while(!ERR_EXIT_STATUS);

  readToken->line = readLineNumber;
  return readToken;
}

void State_InlineComment() {
  do {
    readLastChar = getchar();
  } while (readLastChar != '\n' && readLastChar != EOF);
  repeatLastChar = 1;
}

void State_DivOrMultilineComment() {
  if( (readLastChar=getchar())=='\'' ) {State_MultilineComment();
  }
  else {
    repeatLastChar = 1;
    readToken->type = TK_DIV;
  }
}

void State_MultilineComment() {
  do {
    readLastChar=getchar();
    if( readLastChar==EOF ) {
      // ERR_LEX
      CallError(ERR_LEX);
      repeatLastChar = 1;
      return;
    }
    if( readLastChar=='\'' ) {
      do {
        readLastChar=getchar();
        if( readLastChar==EOF ) {
          // ERR_LEX
          CallError(ERR_LEX);
          repeatLastChar = 1;
          return;
        }

        if( readLastChar=='\n' ) 
          readLineNumber++;

      } while( readLastChar=='\'' );
    }

  } while( readLastChar!='/' );
}

void State_ID() {
  while( isalnum(readLastChar=getchar()) || readLastChar == '_' ) {
    //StringAdd(s,readLastChar);
  }
  repeatLastChar = 1;
  readToken->type = TK_ID;
}


void State_Number() {
  while( isdigit(readLastChar=getchar()) ) {
    //StringAdd(s,readLastChar);
  }
  if( readLastChar=='.' ) {
    //StringAdd(s,readLastChar);
    State_Double();
  }
  else if( readLastChar=='E' || readLastChar=='e' ) {
    //StringAdd(s,readLastChar);
    State_CanDoubleE();
  }
  else {
    repeatLastChar = 1;
    readToken->type = TK_NUM_INTEGER;
  }
}

void State_Double() {
  // Je za teckou aspon jedna cifra?
  if( !isdigit(readLastChar=getchar()) ) {
    repeatLastChar = 1;
    // ERR_LEX
    CallError(ERR_LEX);
    return;
  }
  //StringAdd(s,readLastChar);
  
  while( isdigit(readLastChar=getchar()) ) {
    //StringAdd(s,readLastChar);
  }
  if( readLastChar=='E' || readLastChar=='e' ) {
    //StringAdd(s,readLastChar);
    State_CanDoubleE();
  }
  else {
    repeatLastChar = 1;
    readToken->type = TK_NUM_DOUBLE;
  }
}

void State_CanDoubleE() {
  readLastChar=getchar();
  if( readLastChar=='+' || readLastChar=='-' ) {
    //StringAdd(s,readLastChar);
    readLastChar=getchar();
  }

  if( isdigit(readLastChar) ) {
    //StringAdd(s,readLastChar);
    State_DoubleE();
  }
  else {
    // ERR_LEX
    CallError(ERR_LEX);
    repeatLastChar = 1;
  }
}

void State_DoubleE() {
  while( isdigit(readLastChar=getchar()) ) {
    //StringAdd(s,readLastChar);
  }
  readToken->type = TK_NUM_DOUBLE;
  repeatLastChar = 1;
}

void State_ExMark() {
  if( (readLastChar=getchar())=='\'' ) {
    State_String();
  }
  else {
    // ERR_LEX
    CallError(ERR_LEX);
    repeatLastChar=1;
  }
}

void State_String() {
  do {
    readLastChar=getchar();

    if( readLastChar=='\\' ) {
      State_SpecialChar();
    }
    else if( readLastChar=='\'' ) {
      readToken->type = TK_STRING;
      return;
    }
    else if( readLastChar==EOF || readLastChar=='\n' ) {
      // ERR_LEX
      CallError(ERR_LEX);
      repeatLastChar = 1;
      return;
    }
    else {
      //StringAdd(s,readLastChar);
    }
  } while(1);
}

void State_SpecialChar() {
  if( isdigit(readLastChar=getchar()) ) {

  }
  else {
    switch( readLastChar ) {
      case 'n': 
        //StringAdd(s, (char)(10) );
        break;
      case 'r': 
        //StringAdd(s, (char)(10) );
        break;
      case 't': 
        //StringAdd(s, (char)(10) );
        break;
      case '\\': 
        //StringAdd(s, (char)(10) );
        break;
      case '\"': 
        //StringAdd(s, (char)(10) );
        break;
      case '\'': 
        //StringAdd(s, (char)(10) );
        break;
      default:
        // ERR_LEX
        CallError(ERR_LEX);
        break;
    }
  }
}

void State_GreaterThan() {
  if( (readLastChar=getchar())=='=' ) {
    readToken->type = TK_GREATER_EQUAL;
  }
  else {
    readToken->type = TK_GREATER;
    repeatLastChar = 1;
  }
}

void State_LessThan() {
  if( (readLastChar=getchar())=='=' ) {
    readToken->type = TK_LESS_EQUAL;
  }
  else {
    readToken->type = TK_LESS;
    repeatLastChar = 1;
  }
}

#endif

// Soubor: src/parser.c

