//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/scanner.c                //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////


#ifndef SCANNER_C
#define SCANNER_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "error.h"
#include "dstring.h"
#include "conversion.h"

int repeatLastChar = 0;
TToken* readToken = NULL;
int readLastChar;
unsigned long int readLineNumber = 1;
TString* readString = NULL;

TToken *lastToken = NULL;
int repeatLastToken = 0;

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

void RepeatLastToken() {
  repeatLastToken = 1;
}

void ClearScanner() {
  // fprintf(stderr, ">>TOKEN CLEAN\n");
  TokenDestroy(lastToken);
  lastToken = NULL;
  repeatLastToken = 0;
}

TToken* GetNextToken() {
  if( (readToken=TokenInit())==NULL ) {
    // ERR_INTERNAL
    CallError(ERR_INTERNAL);
    return NULL;
  }

  if( repeatLastToken ) { // Vratit naposled nacteny token
    repeatLastToken--;

    *readToken = *lastToken;
    readToken->string = malloc(strlen(lastToken->string)+1);     
    strcpy(readToken->string, lastToken->string);   

    // fprintf(stderr, ">RepeatToke:\n"); // DEBUG
    // PrintToken(readToken);// DEBUG
    return readToken;
  }

  if( (readString = StringInit())==NULL ) {
    // ERR_INTERNAL
    CallError(ERR_INTERNAL);
    return NULL;
  }
  

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
      //String
      StringAdd(readString, tolower(readLastChar) );
      State_ID();
      break;
    }
    else if( isdigit(readLastChar) ) {
      //String
      StringAdd(readString,readLastChar);
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
    else if( readLastChar==',' ) {
      readToken->type = TK_COMMA;
      break;
    }
    else if( CanBeIgnored(readLastChar) ) { ; }
    else {
      // ERR_LEX
      PrintLineNumberErr(readLineNumber);
      fprintf(stderr, "Byl zadan neplaty znak '%c'\n", readLastChar );
      CallError(ERR_LEX);
      break;
    }
  } while(!ERR_EXIT_STATUS);

  readToken->line = readLineNumber;
  Convert(readToken);

  //if(ERR_EXIT_STATUS) PrintLineErr(readToken); // ERR

  /// START Zaloha pro pripadne opakovani tokenu.
  TokenDestroy(lastToken);
  lastToken = TokenInit();
  *lastToken = *readToken; 
  lastToken->string = StringCopy(readString);
  /// END

  StringDestroy(readString);

  // PrintToken(readToken);// DEBUG

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
      PrintLineNumberErr(readLineNumber);
      fprintf(stderr, "Nebyl ocekavan konec souboru.\n");
      CallError(ERR_LEX);
      repeatLastChar = 1;
      return;
    }
    if( readLastChar=='\'' ) {
      do {
        readLastChar=getchar();
        if( readLastChar==EOF ) {
          // ERR_LEX
          PrintLineNumberErr(readLineNumber);
          fprintf(stderr, "Nebyl ocekavan konec souboru.\n" );
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
    //String
    StringAdd(readString,tolower(readLastChar) );
  }
  repeatLastChar = 1;
  readToken->type = TK_ID;
  readToken->string = StringCopy(readString);
}


void State_Number() {
  while( isdigit(readLastChar=getchar()) ) {
    //String
    StringAdd(readString,readLastChar);
  }
  if( readLastChar=='.' ) {
    //String
    StringAdd(readString,readLastChar);
    State_Double();
  }
  else if( readLastChar=='E' || readLastChar=='e' ) {
    //String
    StringAdd(readString,readLastChar);
    State_CanDoubleE();
  }
  else {
    repeatLastChar = 1;
    readToken->type = TK_NUM_INTEGER;
    readToken->string = StringCopy(readString);
  }
}

void State_Double() {
  // Je za teckou aspon jedna cifra?
  if( !isdigit(readLastChar=getchar()) ) {
    repeatLastChar = 1;
    // ERR_LEX
    PrintLineNumberErr(readLineNumber);
    fprintf(stderr, "Byla ocekavana cislice za teckou misto '%c'\n", readLastChar );
    CallError(ERR_LEX);
    return;
  }
  //String
  StringAdd(readString,readLastChar);
  
  while( isdigit(readLastChar=getchar()) ) {
    //String
    StringAdd(readString,readLastChar);
  }
  if( readLastChar=='E' || readLastChar=='e' ) {
    //String
    StringAdd(readString,readLastChar);
    State_CanDoubleE();
  }
  else {
    repeatLastChar = 1;
    readToken->type = TK_NUM_DOUBLE;
    readToken->string = StringCopy(readString);
  }
}

void State_CanDoubleE() {
  readLastChar=getchar();
  if( readLastChar=='+' || readLastChar=='-' ) {
    //String
    StringAdd(readString,readLastChar);
    readLastChar=getchar();
  }

  if( isdigit(readLastChar) ) {
    //String
    StringAdd(readString,readLastChar);
    State_DoubleE();
  }
  else {
    // ERR_LEX
    PrintLineNumberErr(readLineNumber);
    fprintf(stderr, "Byla ocekavana cislice exponentu misto '%c'\n", readLastChar );
    CallError(ERR_LEX);
    repeatLastChar = 1;
  }
}

void State_DoubleE() {
  while( isdigit(readLastChar=getchar()) ) {
    //String
    StringAdd(readString,readLastChar);
  }
  repeatLastChar = 1;
  readToken->type = TK_NUM_DOUBLE;
  readToken->string = StringCopy(readString);
}

void State_ExMark() {
  if( (readLastChar=getchar())=='\"' ) {
    State_String();
  }
  else {
    // ERR_LEX
    PrintLineNumberErr(readLineNumber);
    fprintf(stderr, "Za vykricnikem nemuze nasledovat '%c'\n", readLastChar );
    CallError(ERR_LEX);
    repeatLastChar=1;
  }
}

void State_String() {
  do {
    readLastChar=getchar();

    if( readLastChar=='\\' ) {
      State_SpecialChar();
      if(ERR_EXIT_STATUS) {
        return;
      }
    }
    else if( readLastChar=='\"' ) {
      readToken->type = TK_NUM_STRING;
      readToken->string = StringCopy(readString);
      return;
    }
    else if( readLastChar==EOF || readLastChar=='\n' ) {
      // ERR_LEX
      PrintLineNumberErr(readLineNumber);
      fprintf(stderr, "Textovy retezec nesmi obsahovat odradkovani.\n" );
      CallError(ERR_LEX);
      repeatLastChar = 1;
      return;
    }
    else {
      //String
      StringAdd(readString,readLastChar);
    }
  } while(1);
}

void State_SpecialChar() {
  if( isdigit(readLastChar=getchar()) ) {
    // \0 
    TString* tmpString;
    tmpString = StringInit();
    StringAdd(tmpString, readLastChar);

    if( isdigit(readLastChar=getchar()) ) {
      // \00
      StringAdd(tmpString, readLastChar);
      if( isdigit(readLastChar=getchar()) ) {
        // \000
        StringAdd(tmpString, readLastChar);
        int tmpChar = atoi(tmpString->string);
        if( tmpChar >= 1 && tmpChar <= 255 ) {
          // String
          StringAdd(readString, (char)tmpChar );
        }
        else {
          // LEX_ERR
          PrintLineNumberErr(readLineNumber);
          fprintf(stderr, "V escape sekvenci bylo zadano neplatne trimistne cislo.\n" );
          CallError(ERR_LEX);
        }
      }
      else {
        // LEX_ERR
        PrintLineNumberErr(readLineNumber);
        fprintf(stderr, "V escape sekvenci bylo zadano neplatne trimistne cislo.\n" );
        CallError(ERR_LEX);
      }
    }
    else {
      // LEX_ERR
      PrintLineNumberErr(readLineNumber);
      fprintf(stderr, "V escape sekvenci bylo zadano neplatne trimistne cislo.\n" );
      CallError(ERR_LEX);
    }

    StringDestroy(tmpString);
  }
  else {
    switch( readLastChar ) {
      case 'n': 
        //String
        StringAdd(readString, '\n' );
        break;
      case 'r': 
        //String
        StringAdd(readString, '\r' );
        break;
      case 't': 
        //String
        StringAdd(readString, '\t' );
        break;
      case '\\': 
        //String
        StringAdd(readString, '\\' );
        break;
      case '\"': 
        //String
        StringAdd(readString, '\"' );
        break;
      case '\'': 
        //String
        StringAdd(readString, '\'' );
        break;
      default:
        // ERR_LEX
        CallError(ERR_LEX);
        StringAdd(readString,readLastChar);
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
  else if( readLastChar == '>' ) {
    readToken->type = TK_NOT_EQUAL;
  }
  else {
    readToken->type = TK_LESS;
    repeatLastChar = 1;
  }
}


// Funkci je mozne volat v main.
// Pro oskouseni zpracovavani vstupu do vystupnich tokenu.
// int main() {
//   TestScanner();
// }
void TestScanner() {
  TTokenType tmp=0;
  printf("Token Test - Ctrl+d = EOF \n");
  do {
    TToken* tkn = GetNextToken();
    //PrintTokenType(tkn->type);
    PrintToken(tkn);
    tmp = tkn->type; 
    TokenDestroy(tkn);
  } while(tmp != TK_EOF && !ERR_EXIT_STATUS);
}

#endif

// Soubor: src/scanner.c

