//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/parser2.c                //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef PARSER2_C
#define PARSER2_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "error.h"
#include "dstring.h"
#include "symtable.h"
#include "list.h"
#include "parser2.h"

symtable_t *GlobalSymtable; // Globalni tabulka symbolu alias Tabulka fuknci

// Znici stary token a vrati novy
TToken* GetNextDestroyOldToken(TToken *tkn, int canGetEOL) {
  do {
    TokenDestroy( tkn );
    tkn = GetNextToken();
  } while( !canGetEOL &&  tkn->type == TK_EOL );
  return tkn;
}

int SyntaxStartParse() {
  SymtableInit(&GlobalSymtable);
  TToken *tmpToken = NULL;
  TToken** tkn = &tmpToken;

  (*tkn) = GetNextToken();
  if( Syntaxx_Program(tkn) ) {
  }
  else {
    // ERR_SYN
    CallError(ERR_SYN);
  }

  TokenDestroy( (*tkn) );
  SymtableFree(GlobalSymtable);

  return !ERR_EXIT_STATUS;
}

int Syntaxx_Program(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_DECLARE:
    case TK_FUNCTION:
    case TK_SCOPE:
    case TK_EOF:
      return Syntaxx_ListDecDef(tkn) && Syntaxx_ScopeDef(tkn);
      break;
    default:
      return 0;
      break;
  }
}

int Syntaxx_ListDecDef(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_DECLARE:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_FunctionHead(tkn) && Syntaxx_ListDecDef(tkn);
      break;
    case TK_FUNCTION:
      return Syntaxx_FunctionHead(tkn) && Syntaxx_FunctionBody(tkn) && Syntaxx_FunctionEnd(tkn) && Syntaxx_ListDecDef(tkn);
      break;
    case TK_SCOPE:
      return 1;
      break;
    default:
      return 0;
      break;
  }
}

int Syntaxx_FunctionHead(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_FUNCTION:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_BRACKET_ROUND_LEFT )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_ListParam(tkn) )
        return 0;
      if( (*tkn)->type != TK_BRACKET_ROUND_RIGHT )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL )
        return 0;
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ListParam(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
      return Syntaxx_Param(tkn) && Syntaxx_NextParam(tkn);
      break;
    case TK_BRACKET_ROUND_RIGHT:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_Param(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_AS )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_DataType(tkn);
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_NextParam(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_COMMA:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_Param(tkn) && Syntaxx_NextParam(tkn);
      break;
    case TK_BRACKET_ROUND_RIGHT:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_DataType(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_INTEGER:
    case TK_DOUBLE:
    case TK_STRING:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_FunctionEnd(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_END:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_FUNCTION )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeDef(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_SCOPE:
      return Syntaxx_ScopeHead(tkn) && Syntaxx_FunctionBody(tkn) && Syntaxx_ScopeEnd(tkn);
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeHead(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_SCOPE:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL )
        return 0;
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeEnd(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_END:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_SCOPE )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ScopeAfter(tkn);
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeAfter(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_EOL:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ScopeAfter(tkn);
      break;
    case TK_EOF:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_FunctionBody(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_END:
    case TK_DIM:
    case TK_WHILE:
    case TK_IF:
    case TK_INPUT:
    case TK_PRINT:
      return Syntaxx_ListVarDef(tkn) && Syntaxx_ListCommand(tkn);
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ListVarDef(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_DIM:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_AS )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_DataType(tkn) )
        return 0;
      if( (*tkn)->type != TK_EOL )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ListVarDef(tkn); 
      break;
    case TK_ID:
    case TK_END:
    case TK_WHILE:
    case TK_IF:
    case TK_INPUT:
    case TK_PRINT:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ListCommand(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_WHILE:
    case TK_IF:
    case TK_INPUT:
    case TK_PRINT:
      if( !Syntaxx_Command(tkn) )
        return 0;
      if( (*tkn)->type != TK_EOL )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ListCommand(tkn);
      break;
    case TK_END:
    case TK_LOOP:
    case TK_ELSE:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_Command(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EQUAL )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_Assignment(tkn);
      break;
    case TK_WHILE:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Condition(tkn) ) 
        return 0;
      if( (*tkn)->type != TK_EOL )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_ListCommand(tkn) )
        return 0;
      if( (*tkn)->type != TK_LOOP )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    case TK_IF:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Condition(tkn) )
        return 0;
      if( (*tkn)->type != TK_EOL )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_ListCommand(tkn) )
        return 0;
      if( (*tkn)->type != TK_ELSE )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_ListCommand(tkn) )
        return 0;
      if( (*tkn)->type != TK_END )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_IF )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    case TK_INPUT:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
    case TK_PRINT:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ListExpression(tkn);
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ListExpression(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_NA: // TK_EXPRESSION
      if( !Syntaxx_Expression(tkn) )
        return 0;
      if( (*tkn)->type != TK_SEMICOLON )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ListExpression(tkn);
      break;
    case TK_EOL:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_Condition(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_NA: // TK_EXPRESSION
      if( !Syntaxx_Expression(tkn) )
        return 0;
      if( !Syntaxx_RO(tkn) )
        return 0;
      if( !Syntaxx_Expression(tkn) )
        return 0;
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_RO(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_EQUAL:
    case TK_LESS:
    case TK_GREATER:
    case TK_NOT_EQUAL:
    case TK_LESS_EQUAL:
    case TK_GREATER_EQUAL:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_Assignment(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_BRACKET_ROUND_LEFT )
        return 0;
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_ListInParam(tkn) )
        return 0;
      if( (*tkn)->type != TK_BRACKET_ROUND_RIGHT )
        return 0;
      return 1;
      break;
    case TK_NA: // TK_EXPRESSION
      return Syntaxx_Expression(tkn);
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_ListInParam(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
      return Syntaxx_InParam(tkn) && Syntaxx_NextInParam(tkn);
      break;
    case TK_BRACKET_ROUND_RIGHT:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_InParam(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
      return Syntaxx_Term(tkn);
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_NextInParam(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_COMMA:
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_InParam(tkn) && Syntaxx_NextInParam(tkn);
      break;
    case TK_BRACKET_ROUND_RIGHT:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}
int Syntaxx_Term(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}

int Syntaxx_Expression(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_NA:
      return 1;
      break;
    default:
      return 0;
      break;
  }
  
}

#endif

// Soubor: src/parser2.c

