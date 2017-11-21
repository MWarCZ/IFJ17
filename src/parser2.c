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
    if( tkn )
      TokenDestroy( tkn );
    tkn = GetNextToken();
  } while( !canGetEOL &&  tkn->type == TK_EOL );
  return tkn;
}

int SyntaxStartParse() {
  SymtableInit(&GlobalSymtable);
  TToken *tmpToken = NULL;
  TToken** tkn = &tmpToken;

  //(*tkn) = GetNextToken();
  (*tkn) = GetNextDestroyOldToken( (*tkn),0 );

  if( Syntaxx_Program(tkn) ) {
  }
  else {
    // ERR_SYN
    CallError(ERR_SYN);
  }

  TokenDestroy( (*tkn) );
  SymtableFree(GlobalSymtable);
  ClearScanner();

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
      fprintf(stderr, "Program\n");
      return 0;
      break;
  }
}

int Syntaxx_ListDecDef(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_DECLARE: /// declare
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
      fprintf(stderr, "ListDecDef\n");
      return 0;
      break;
  }
}

int Syntaxx_FunctionHead(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_FUNCTION: /// function
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID ) { /// id
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_BRACKET_ROUND_LEFT ) { /// (
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 ); 
      if( !Syntaxx_ListParam(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_BRACKET_ROUND_RIGHT ) { /// )
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_AS ) { /// as
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_DataType(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return 1;
      break;
    default:
      fprintf(stderr, "FunctionHead\n");
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
      fprintf(stderr, "ListParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Param(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID: /// id
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_AS ) { /// as
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_DataType(tkn);
      break;
    default:
      fprintf(stderr, "Param\n");
      return 0;
      break;
  }
  
}
int Syntaxx_NextParam(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_COMMA: /// ,
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_Param(tkn) && Syntaxx_NextParam(tkn);
      break;
    case TK_BRACKET_ROUND_RIGHT:
      return 1;
      break;
    default:
      fprintf(stderr, "NextParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_DataType(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_INTEGER: /// integer
    case TK_DOUBLE: /// double
    case TK_STRING: /// string
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    default:
      fprintf(stderr, "DataType\n");
      return 0;
      break;
  }
  
}
int Syntaxx_FunctionEnd(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_END: /// end
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_FUNCTION ) { /// function
        fprintf(stderr, "end function\n");
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return 1;
      break;
    default:
      fprintf(stderr, "FunctionEnd\n");
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
      fprintf(stderr, "ScopeDef\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeHead(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_SCOPE: /// scope
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return 1;
      break;
    default:
      fprintf(stderr, "ScopeHead\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeEnd(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_END: /// end
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_SCOPE ) { /// scope
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ScopeAfter(tkn);
      break;
    default:
      fprintf(stderr, "ScopeEnd\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeAfter(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_EOL: /// eol
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return Syntaxx_ScopeAfter(tkn);
      break;
    case TK_EOF: /// eof
      return 1;
      break;
    default:
      fprintf(stderr, "ScopeAfter\n");
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
      fprintf(stderr, "FunctionBody\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ListVarDef(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_DIM: /// dim
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID ) { /// id
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_AS ) { /// as
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_DataType(tkn) ) { 
        return 0;
      }
      if( !Syntaxx_VarDefAssigment(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
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
      fprintf(stderr, "ListVarDef\n");
      return 0;
      break;
  }
  
}

int Syntaxx_VarDefAssigment(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_EQUAL: /// =
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Expression(tkn) ) { // TK_EXPRESSION
        return 0;
      }
      return 1;
      break;
    case TK_EOL:
      return 1;
      break;
    default:
      fprintf(stderr, "VarDefAssigment\n");
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
      if( !Syntaxx_Command(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return Syntaxx_ListCommand(tkn);
      break;
    case TK_END:
    case TK_LOOP:
    case TK_ELSE:
      return 1;
      break;
    default:
      fprintf(stderr, "ListCommand\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Command(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID: /// id
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EQUAL ) { /// =
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_Assignment(tkn);
      break;
    case TK_WHILE: /// while
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Condition(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      if( !Syntaxx_ListCommand(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_LOOP ) { /// loop
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    case TK_IF: /// if
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Condition(tkn) ) { 
        return 0;
      }
      if( (*tkn)->type != TK_THEN ) { /// then
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      if( !Syntaxx_ListCommand(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_ELSE ) { /// else
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      if( !Syntaxx_ListCommand(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_END ) { /// end
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_IF ) { /// if
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    case TK_INPUT: /// input
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID ) { /// id
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
    case TK_PRINT: /// print
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ListExpression(tkn);
      break;
    default:
      fprintf(stderr, "Command\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ListExpression(TToken **tkn) {
  // Expression
  if( (*tkn)->type == TK_ID || (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING ) {
    if( Syntaxx_Expression(tkn) ) {
      if( (*tkn)->type != TK_SEMICOLON ) {
        fprintf(stderr, "semicolon\n");
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ListExpression(tkn);
    }
  }
  switch( (*tkn)->type ) {
    case TK_EOL: 
      return 1;
      break;
    default:
      fprintf(stderr, "ListExpression\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Condition(TToken **tkn) {
  // Expression
  if( (*tkn)->type == TK_ID || (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING ) {
    if( !Syntaxx_Expression(tkn) ) {
      return 0;
    } 
    if( !Syntaxx_RO(tkn) ) {
      return 0;
    }
    if( !Syntaxx_Expression(tkn) ) {
      return 0;
    }
    return 1;
  }
  fprintf(stderr, "Condition\n");
  return 0;
  
}
int Syntaxx_RO(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_EQUAL: /// =
    case TK_LESS: /// <
    case TK_GREATER: /// >
    case TK_NOT_EQUAL: /// <>
    case TK_LESS_EQUAL: /// <=
    case TK_GREATER_EQUAL: /// >=
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    default:
      fprintf(stderr, "RO\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Assignment(TToken **tkn) {
  // Expression
  if( (*tkn)->type == TK_ID || (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING ) {
    if( Syntaxx_Expression(tkn) ) {
      return 1;
    } // Pokud se nepovede tak nedelej nic
  }
  switch( (*tkn)->type ) {
    case TK_ID: /// id
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_BRACKET_ROUND_LEFT ) { /// (
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_ListInParam(tkn) ) {
        return 0;
      }
      if( (*tkn)->type != TK_BRACKET_ROUND_RIGHT ) { /// )
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    default:
      fprintf(stderr, "Assignment\n");
      return 0;
      break;
  }
  return 0;
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
      fprintf(stderr, "ListInParam\n");
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
      fprintf(stderr, "InParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_NextInParam(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_COMMA: /// ,
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_InParam(tkn) && Syntaxx_NextInParam(tkn);
      break;
    case TK_BRACKET_ROUND_RIGHT:
      return 1;
      break;
    default:
      fprintf(stderr, "NextInParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Term(TToken **tkn) {
  switch( (*tkn)->type ) {
    case TK_ID: /// id
    case TK_NUM_INTEGER: /// int 5
    case TK_NUM_DOUBLE: /// float 2.3
    case TK_NUM_STRING: /// str !"string"
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    default:
      fprintf(stderr, "Term\n");
      return 0;
      break;
  }
  
}




//----------------------------------------
// Syntaxx_Expression
// ---------------------------------------

/*_____________
  |       |   |
  |   * / | 1 |
  | \ mod | 2 |
  |   + - | 3 |
  |     ( | 4 |
  |_______|___|
 */
int PriorityOperator(TTokenType type) {
  switch(type) {
    case TK_BRACKET_ROUND_LEFT:
      return 4;
    case TK_PLUS:
    case TK_MINUS:
      return 3;
    case TK_MUL:
    case TK_DIV:
      return 1;
    case TK_MOD:
    case TK_DIV_INT:
      return 2;
    default:
      return -1;
  }
}

// Prevod vyrazu z infix na posfix - Muze byt operator vlozen na zasobnik operatoru?
int CanBeOperatorPush(TTokenType operatorNow, TTokenType operatorOnStack ) {
  if( operatorNow == TK_BRACKET_ROUND_LEFT ) 
    return 1;
  if( PriorityOperator(operatorNow) < PriorityOperator(operatorOnStack) ) 
    return 1;
  return 0;
}

// Prevod vyrazu z infix na posfix - Muze nasledovat token ... za tokenem ...?
int CanBeTokenAfterToken(TTokenType now, TTokenType last) {
  switch(last) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
    case TK_BRACKET_ROUND_RIGHT:
      switch(now) {
        case TK_PLUS:
        case TK_MINUS:
        case TK_MUL:
        case TK_DIV:
        case TK_MOD:
        case TK_DIV_INT:
        case TK_BRACKET_ROUND_RIGHT:
          return 1;
        default:
          return 0;
      }
    case TK_BRACKET_ROUND_LEFT:
    case TK_PLUS:
    case TK_MINUS:
    case TK_MUL:
    case TK_DIV:
    case TK_MOD:
    case TK_DIV_INT:
      switch(now) {
        case TK_ID:
        case TK_NUM_INTEGER:
        case TK_NUM_DOUBLE:
        case TK_NUM_STRING:
        case TK_BRACKET_ROUND_LEFT:
          return 1;
        default:
          return 0;
      }
    default:
      break;
  }
  return 0;
}

int CanBeTokenInExpression(TTokenType type) {
  switch(type) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
    case TK_BRACKET_ROUND_RIGHT:
    case TK_BRACKET_ROUND_LEFT:
    case TK_PLUS:
    case TK_MINUS:
    case TK_MUL:
    case TK_DIV:
    case TK_MOD:
    case TK_DIV_INT:
      return 1;
      break;
    default:
      return 0;
      break;
  }

}

int Syntaxx_Expression( TToken **tkn)  {
  int returnValue = 1; // Navratova hodnota
  TList *StackOperator; // Stack Tokenu
  TList *ListPostFix; // List Tokenu
  StackOperator = ListInit();
  ListPostFix = ListInit();

  if(StackOperator == NULL || ListPostFix == NULL) {
    // ERR_INTERNAL
    CallError(ERR_INTERNAL);   
    return 0;
  }

  TTokenType lastToken = TK_BRACKET_ROUND_LEFT;

  do {

    if( CanBeTokenInExpression( (*tkn)->type ) ) {
      if( !CanBeTokenAfterToken( (*tkn)->type, lastToken ) ) {
        // neocekavany token vyrazu
        fprintf(stderr, ">Expression: Neocekavany token, ale nejedna se o chybu.\n");// DEBUG
        returnValue = 0;
        RepeatLastToken();
        TListData data;
        if( ListPopBack(ListPostFix, &data) ) {
          (*tkn) = data.pointer;
        }
        break; 
      }
    }
    lastToken =  (*tkn)->type;

    if(  (*tkn)->type == TK_ID ) {
      TListData data;
      data.pointer = (*tkn);
      ListPushBack(ListPostFix, data);
    }
    else if( (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING ) {
      TListData data;
      data.pointer = (*tkn);
      ListPushBack(ListPostFix, data);
    }
    else if(  (*tkn)->type == TK_BRACKET_ROUND_LEFT ) {
      TListData data;
      data.pointer = (*tkn);
      ListPush(StackOperator, data);
    }
    else if(  (*tkn)->type == TK_PLUS ||  (*tkn)->type == TK_MINUS ||  (*tkn)->type == TK_MUL ||  (*tkn)->type == TK_DIV ||  (*tkn)->type == TK_DIV_INT ||  (*tkn)->type == TK_MOD ) {
      // Podle priority operatoru zpracuj operatory
      TListData data;
      while( ListFront(StackOperator, &data) && !CanBeOperatorPush( (*tkn)->type, ( (TToken*)data.pointer)->type ) ) {
        ListPushBack(ListPostFix, data);
        ListPop(StackOperator, &data);
      }//- while
      data.pointer = (*tkn);
      ListPush(StackOperator, data);
    }
    else if(  (*tkn)->type == TK_BRACKET_ROUND_RIGHT ) {
      TListData data;
      while(1) {
        if( !ListPop(StackOperator, &data) ) {
          // Pokud zasobnik op Neobsahuje '(' 
          // ERR_SYN
          fprintf(stderr, ">Expression: Vyraz obsahuje prebytecnou zavorku ')'\n");
          CallError(ERR_SYN);
          break;
        }
        else if( ((TToken*)data.pointer)->type == TK_BRACKET_ROUND_LEFT ) {
          break;
        }
        else {
          ListPushBack(ListPostFix, data); // dokud nenajde '(' tak operace sklada na vystupni retezec
        }

      }//-while
    }
    else {
      TListData data;
      while( ListPop(StackOperator, &data) ) {
        if( ((TToken*)data.pointer)->type == TK_BRACKET_ROUND_LEFT ) {
          // ERR_SYN
          fprintf(stderr, ">Expression: Neocekavana zavorka '('.\n");
          CallError(ERR_SYN);
          break;
        }
        else {
          ListPushBack(ListPostFix, data);
        }
      }//-while
      break; // Uspesny konec
    }

    (*tkn) = GetNextToken(); // Jen novy token ale stary neuvolnovat nebot je v ListPostFix nebo v StackOperator
  } while(!ERR_EXIT_STATUS);


  if( returnValue && ListEmpty(ListPostFix) ) { // Pokud je vystupni Vyraz prazdny, tak neexistoval
    fprintf(stderr, ">Expression: Nebyl zadan vyraz.\n");
    CallError(ERR_SYN);
  }
  if( returnValue && !ListEmpty(StackOperator) ) {
    fprintf(stderr, ">Expression: Nespravny vyraz zkontrolujte pocet operandu a operatoru.\n");
    CallError(ERR_SYN);
  }

  // ListPostFix obsahuje vyraz v posix podobe ktery je potreba vygenerovat
  


  // Uvolneni tokenu a debugovaci vypis dat
  TListData data;

  fprintf(stderr, ">>>ListPostFix\n");// DEBUG
  while( ListPop(ListPostFix, &data) ) {
    PrintToken( ((TToken*)data.pointer) );// DEBUG
    TokenDestroy( ((TToken*)data.pointer) );
  }
  fprintf(stderr, ">>>StackOperator\n");// DEBUG
  while( ListPop(StackOperator, &data) ) {
    PrintToken( ((TToken*)data.pointer) );// DEBUG
    TokenDestroy( ((TToken*)data.pointer) );
  }
  fprintf(stderr, ">>>\n");// DEBUG

  ListDestroy(StackOperator);
  ListDestroy(ListPostFix);

  return !ERR_EXIT_STATUS && returnValue;
} //- int Syntaxx_Expression


#endif

// Soubor: src/parser2.c

